//
// Created by Jackson Campolattaro on 1/16/23.
//

#ifndef N_BODY_DUALTREESOLVER_H
#define N_BODY_DUALTREESOLVER_H

#include "../Solver.h"

#include "Trees/LinearBVH.h"
#include "Trees/FieldOctree.h"
#include "Trees/DescentCriterion.h"
#include <tbb/parallel_for_each.h>

#include <span>
#include <memory>


namespace NBody {

    template<typename ActiveTree, typename PassiveTree, typename DescentCriterionType>
    class DualTreeSolver : public Solver {
    private:

        ActiveTree _activeTree;
        PassiveTree _passiveTree;
        DescentCriterionType _descentCriterion{0.4f};

    public:

        DualTreeSolver(Simulation &simulation, Physics::Rule &rule) :
                Solver(simulation, rule),
                _activeTree(simulation),
                _passiveTree(simulation) {
            _passiveTree.maxDepth() = 32;
            _passiveTree.maxLeafSize() = 16;
        }

        const ActiveTree &activeTree() const { return _activeTree; }

        ActiveTree &activeTree() { return _activeTree; }

        const PassiveTree &passiveTree() const { return _passiveTree; }

        PassiveTree &passiveTree() { return _passiveTree; }

        float &theta() { return _descentCriterion.theta(); }

        const float &theta() const { return _descentCriterion.theta(); }

        void step() override {

            auto targets = _simulation.view<const Position, const Mass, Velocity, const PassiveTag>();
            auto movableTargets = _simulation.view<Position, const Mass, const Velocity>();

            _statusDispatcher.emit({"Building active tree"});
            _activeTree.refine();

            _statusDispatcher.emit({"Building passive tree"});
            _passiveTree.refine();

            {
                _statusDispatcher.emit({"Resetting forces"});
                auto view = _simulation.view<const Position, const PassiveTag>();
                for (const Entity e: view)
                    _simulation.emplace_or_replace<Force>(e, 0.0f, 0.0f, 0.0f);
            }

            {
                _statusDispatcher.emit({"Computing forces"});
                auto startingNodes =
                        loadBalancedSplit(_passiveTree, 64);
                tbb::parallel_for_each(startingNodes, [&](std::reference_wrapper<typename PassiveTree::Node> node) {
                    computeForces(
                            _activeTree.root(),
                            node
                    );
                });
            }

            {
                _statusDispatcher.emit({"Collapsing forces"});
                auto view = _simulation.view<const Mass, Force>();
                _passiveTree.root().collapseForces(view);
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
                const typename ActiveTree::Node &activeNode,
                typename PassiveTree::Node &passiveNode) {

            // If either node is empty, we have no need to calculate forces between them
            if (activeNode.contents().empty() || passiveNode.contents().empty())
                return;

            // If the nodes are far enough apart or both leaves, we can use their summaries
            if (_descentCriterion(activeNode, passiveNode)) {

                // node-node interaction
                passiveNode.force() += (glm::vec3) _rule(activeNode.centerOfMass(), activeNode.totalMass(),
                                                         passiveNode.center(), passiveNode.totalMass());

            } else if (activeNode.isLeaf() && passiveNode.isLeaf()) {

                // If both nodes are leaves & weren't far enough to summarize, then compute individual interactions
                for (auto activeParticle: activeNode.contents()) {
                    for (auto passiveParticle: passiveNode.contents()) {

                        _simulation.template get<Force>(passiveParticle) +=
                                (glm::vec3) _rule(_simulation.get<const Position>(activeParticle),
                                                  _simulation.get<const Mass>(activeParticle),
                                                  _simulation.get<const Position>(passiveParticle),
                                                  _simulation.get<const Mass>(passiveParticle));

                    }

                    // todo: direct particle-particle interaction might not be necessary
                    //                    passiveNode.force() += (glm::vec3) _rule(activeParticles.get<const Position>(activeParticle),
                    //                                                             activeParticles.get<const Mass>(activeParticle),
                    //                                                             passiveNode.center(), passiveNode.totalMass());
                }

            } else {

                // If the passive node isn't a leaf, we'll descend all its children
                std::span<typename PassiveTree::Node> passiveNodesToDescend =
                        passiveNode.isLeaf() ? std::span<typename PassiveTree::Node>{&passiveNode, 1}
                                             : passiveNode.children();

                // If the active node isn't a leaf, we'll descend all its children
                std::span<const typename ActiveTree::Node> activeNodesToDescend =
                        activeNode.isLeaf() ? std::span<const typename ActiveTree::Node>{&activeNode, 1}
                                            : activeNode.children();

                // Treat every combination of force & field node
                for (auto &childPassiveNode: passiveNodesToDescend) {
                    for (const auto &childActiveNode: activeNodesToDescend) {
                        computeForces(childActiveNode,
                                      childPassiveNode);
                    }
                }
            }
        }

    };

    class MVDRSolver : public DualTreeSolver<LinearBVH, FieldOctree, DescentCriterion::DiagonalOverDistance> {
    public:

        using DualTreeSolver::DualTreeSolver;

        std::string id() override { return "mvdr"; };

        std::string name() override { return "Mark van de Ruit"; };

        int &passiveTreeMaxDepth() { return passiveTree().maxDepth(); }

        const int &passiveTreeMaxDepth() const { return passiveTree().maxDepth(); }

        int &passiveTreeMaxLeafSize() { return passiveTree().maxLeafSize(); }

        const int &passiveTreeMaxLeafSize() const { return passiveTree().maxLeafSize(); }
    };
}

#endif //N_BODY_DUALTREESOLVER_H
