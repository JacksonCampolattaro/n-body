//
// Created by Jackson Campolattaro on 1/26/23.
//

#ifndef N_BODY_DUALTRAVERSALSOLVER_H
#define N_BODY_DUALTRAVERSALSOLVER_H

#include <NBody/Simulation/Solver.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/lockstepDualTree.h>

namespace NBody {

    template<typename DualTree, DescentCriterionType DescentCriterion>
    class DualTraversalSolver : public Solver {
    private:

        DualTree _tree;

        DescentCriterion _descentCriterion{0.4};

    public:

        DualTraversalSolver(Simulation &simulation, Physics::Rule &rule) :
                Solver(simulation, rule),
                _tree(simulation) {}

        float &theta() { return _descentCriterion.theta(); }

        const float &theta() const { return _descentCriterion.theta(); }

        const DualTree &tree() const { return _tree; }

        DualTree &tree() { return _tree; }

        void updateAccelerations() override {

            {
                _statusDispatcher.emit({"Building dual tree"});
                _tree.refine();
            }

            {
                _statusDispatcher.emit({"Resetting accelerations"});
                auto view = _simulation.template view<Acceleration>();
                view.each([](Acceleration &acceleration) { acceleration = {0.0f, 0.0f, 0.0f}; });
            }

            {
                _statusDispatcher.emit({"Computing accelerations"});
                auto startingNodes = _tree.loadBalancedBreak(256);
                tbb::parallel_for_each(startingNodes, [&](std::reference_wrapper<typename DualTree::Node> node) {
                    Descent::lockstepDualTree(_tree.root(), node.get(),
                                              _descentCriterion, _rule,
                                              _simulation.template view<const Position, const Mass>(),
                                              _simulation.template view<const Position, Acceleration>()
                    );
                    //                    computeAccelerations(
                    //                            _simulation.template view<const Position, Acceleration>(),
                    //                            _simulation.template view<const Position, const Mass>(),
                    //                            _tree.root(),
                    //                            node
                    //                    );
                });

                // This seems like it should perform better, but it actually does worse
                //                auto startingNodes = _tree.depthBreak(4);
                //                tbb::parallel_for_each(startingNodes, [&](std::reference_wrapper<typename DualTree::Node> node) {
                //                    for (auto &otherNode : startingNodes) {
                //                        computeAccelerations(
                //                                _simulation.template view<const Position, Acceleration>(),
                //                                _simulation.template view<const Position, const Mass>(),
                //                                otherNode,
                //                                node
                //                        );
                //                    }
                //                });
            }

            {
                _statusDispatcher.emit({"Collapsing accelerations"});
                auto view = _simulation.template view<const Position, Acceleration>();
                // todo: find a better way of avoiding ambiguity
                Descent::collapseAccelerations(
                        _tree.root(), view,
                        typename DualTree::Node::Summary::Acceleration{}
                );
            }
        }

    };
}

#endif //N_BODY_DUALTRAVERSALSOLVER_H
