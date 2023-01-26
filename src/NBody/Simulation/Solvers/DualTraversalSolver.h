//
// Created by Jackson Campolattaro on 1/26/23.
//

#ifndef N_BODY_DUALTRAVERSALSOLVER_H
#define N_BODY_DUALTRAVERSALSOLVER_H

#include "../Solver.h"

#include "Trees/DualOctree.h"
#include "Trees/DescentCriterion.h"
#include <tbb/parallel_for_each.h>

#include <span>
#include <memory>


namespace NBody {

    template<typename DualTree, typename DescentCriterionType>
    class DualTraversalSolver : public Solver {
    private:

        DualTree _tree;
        DescentCriterionType _descentCriterion{0.4f};

    public:

        DualTraversalSolver(Simulation &simulation, Physics::Rule &rule) :
                Solver(simulation, rule),
                _tree(simulation) {}

        const DualTree &tree() const { return _tree; }

        DualTree &tree() { return _tree; }

        float &theta() { return _descentCriterion.theta(); }

        const float &theta() const { return _descentCriterion.theta(); }

        void step() override {

            auto targets = _simulation.view<const Position, const Mass, Velocity, const PassiveTag>();
            auto movableTargets = _simulation.view<Position, const Mass, const Velocity>();

            {
                _statusDispatcher.emit({"Building dual tree"});
                _tree.refine();
            }

            {
                _statusDispatcher.emit({"Resetting forces"});
                auto view = _simulation.view<const Position, const PassiveTag>();
                for (const Entity e: view)
                    _simulation.emplace_or_replace<Force>(e, 0.0f, 0.0f, 0.0f);
            }

            {
                _statusDispatcher.emit({"Computing forces"});
                auto startingNodes = loadBalancedSplit(_tree, 64);
                tbb::parallel_for_each(startingNodes, [&](std::reference_wrapper<typename DualTree::Node> node) {
                    computeForces(
                            _simulation.view<const Position, const Mass, Force, const PassiveTag>(),
                            _simulation.view<const Position, const Mass, const ActiveTag>(),
                            _tree.root(),
                            node
                    );
                });
            }

            {
                _statusDispatcher.emit({"Collapsing forces"});
                auto view = _simulation.view<const Mass, Force>();
                _tree.root().collapseForces(view);
            }

            // Update velocities, based on force
            {
                _statusDispatcher.emit({"Updating velocities"});
                auto view = _simulation.view<const Force, const Mass, Velocity>();
                tbb::parallel_for_each(view, [&](Entity e) {
                    const auto &f = view.template get<const Force>(e);
                    auto &v = view.template get<Velocity>(e);
                    const auto &m = view.template get<const Mass>(e).mass();
                    Acceleration a = f / m;
                    v = v + (a * _dt);
                });
            }

            // Update positions, based on velocity
            {
                // While the solver is modifying simulation values, the simulation should be locked for other threads
                std::scoped_lock l(_simulation.mutex);

                _statusDispatcher.emit({"Updating positions"});
                auto view = _simulation.view<const Velocity, Position>();

                tbb::parallel_for_each(view, [&](Entity e) {
                    const auto &v = view.template get<const Velocity>(e);
                    auto &p = view.template get<Position>(e);
                    p = p + (v * _dt);
                });
            }

        }

    private:

        void computeForces(
                const entt::basic_view<
                        entt::entity, entt::exclude_t<>,
                        const Position, const Mass, Force, const PassiveTag
                > &passiveParticles,
                const entt::basic_view<
                        entt::entity, entt::exclude_t<>,
                        const Position, const Mass, const ActiveTag
                > &activeParticles,
                const typename DualTree::Node &activeNode,
                typename DualTree::Node &passiveNode) {

            // If either node is empty, we have no need to calculate forces between them
            if (activeNode.contents().empty() || passiveNode.contents().empty())
                return;

            // If the nodes are far enough apart or both leaves, we can use their summaries
            if (_descentCriterion(activeNode, passiveNode)) {

                // node-node interaction
                passiveNode.force() += (glm::vec3) _rule(activeNode.centerOfMass(), activeNode.totalActiveMass(),
                                                         passiveNode.center(), passiveNode.totalPassiveMass());

            } else if (activeNode.isLeaf() && passiveNode.isLeaf()) {

                // If both nodes are leaves & weren't far enough to summarize, then compute individual interactions
                for (auto activeParticle: activeNode.contents()) {
                    for (auto passiveParticle: passiveNode.contents()) {

                        passiveParticles.get<Force>(passiveParticle) +=
                                (glm::vec3) _rule(activeParticles.get<const Position>(activeParticle),
                                                  activeParticles.get<const Mass>(activeParticle),
                                                  passiveParticles.get<const Position>(passiveParticle),
                                                  passiveParticles.get<const Mass>(passiveParticle));

                    }

                    // todo: direct particle-particle interaction might not be necessary
                    //                    passiveNode.force() += (glm::vec3) _rule(activeParticles.get<const Position>(activeParticle),
                    //                                                             activeParticles.get<const Mass>(activeParticle),
                    //                                                             passiveNode.center(), passiveNode.totalMass());
                }

            } else {

                // If the passive node isn't a leaf, we'll descend all its children
                std::span<typename DualTree::Node> passiveNodesToDescend =
                        passiveNode.isLeaf() ? std::span<typename DualTree::Node>{&passiveNode, 1}
                                             : passiveNode.children();

                // If the active node isn't a leaf, we'll descend all its children
                std::span<const typename DualTree::Node> activeNodesToDescend =
                        activeNode.isLeaf() ? std::span<const typename DualTree::Node>{&activeNode, 1}
                                            : activeNode.children();

                // Treat every combination of force & field node
                for (auto &childPassiveNode: passiveNodesToDescend) {
                    for (const auto &childActiveNode: activeNodesToDescend) {
                        computeForces(passiveParticles, activeParticles,
                                      childActiveNode, childPassiveNode);
                    }
                }
            }
        }

    };

    class OctreeDualTraversalSolver : public DualTraversalSolver<DualOctree, DescentCriterion::SideLengthOverDistance> {
    public:

        using DualTraversalSolver::DualTraversalSolver;

        std::string id() override { return "octree-dual-traversal"; };

        std::string name() override { return "Octree Dual Traversal"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };
}

#endif //N_BODY_DUALTRAVERSALSOLVER_H