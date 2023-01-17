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
                _passiveTree(simulation) {}

        const ActiveTree &activeTree() const { return _activeTree; }

        ActiveTree &activeTree() { return _activeTree; }

        const PassiveTree &passiveTree() const { return _passiveTree; }

        PassiveTree &passiveTree() { return _passiveTree; }

        float &theta() { return _descentCriterion.theta(); }

        const float &theta() const { return _descentCriterion.theta(); }

        void step() override {

            auto targets = _simulation.view<const Position, const Mass, Velocity, const PassiveTag>();
            auto movableTargets = _simulation.view<Position, const Mass, const Velocity>();

            _statusDispatcher.emit({"Building Active Tree"});
            _activeTree.refine();

            _statusDispatcher.emit({"Building Passive Tree"});
            _passiveTree.refine();

            {
                _statusDispatcher.emit({"Resetting accelerations"});
                auto view = _simulation.view<const Position, const PassiveTag>();
                for (const Entity e : view)
                    _simulation.emplace_or_replace<Acceleration>(e, 0.0f, 0.0f, 0.0f);
            }

            {
                _statusDispatcher.emit({"Computing Accelerations"});
                auto view = _simulation.view<const Position, const Mass, const ActiveTag>();

                std::vector<std::reference_wrapper<typename PassiveTree::Node>> startingNodes =
                        loadBalancedSplit(_passiveTree, 64);

                tbb::parallel_for_each(startingNodes, [&](std::reference_wrapper<typename PassiveTree::Node> node) {
                    computeAccelerations(
                            _activeTree.root(),
                            view,
                            node
                    );
                });
            }

            {
                _statusDispatcher.emit({"Collapsing accelerations"});
                auto view = _simulation.view<Acceleration>();
                _passiveTree.root().collapseAccelerations(view, {0.0f, 0.0f, 0.0f});
            }


            // Update velocities, based on acceleration
            {
                _statusDispatcher.emit({"Updating velocities"});
                auto view = _simulation.view<const Acceleration, Velocity>();
                tbb::parallel_for_each(view, [&](Entity e) {
                    const auto &a = view.template get<const Acceleration>(e);
                    auto &v = view.template get<Velocity>(e);
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

        void computeAccelerations(
                const typename ActiveTree::Node &activeNode,
                const entt::basic_view<
                        entt::entity, entt::exclude_t<>,
                        const Position, const Mass, const ActiveTag
                > &activeParticles,
                typename PassiveTree::Node &passiveNode) {

            // If either node is empty, we have no need to calculate forces between them
            if (activeNode.contents().empty() || passiveNode.contents().empty())
                return;

            // If the nodes are far enough apart or both leaves, we can use their summaries
            if (_descentCriterion(activeNode, passiveNode)) {

                // node-node interaction
                passiveNode.acceleration() += _rule(activeNode.centerOfMass(), activeNode.totalMass(),
                                                    passiveNode.center());

            } else if (activeNode.isLeaf() && passiveNode.isLeaf()) {

                // If both nodes are leaves & weren't far enough to summarize, then compute individual interactions
                for (auto activeParticle: activeNode.contents()) {

                    // todo: maybe iterate over passive particles as well
                    passiveNode.acceleration() += _rule(activeParticles.get<const Position>(activeParticle),
                                                       activeParticles.get<const Mass>(activeParticle),
                                                       passiveNode.center());
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
                        computeAccelerations(childActiveNode,
                                             activeParticles,
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
