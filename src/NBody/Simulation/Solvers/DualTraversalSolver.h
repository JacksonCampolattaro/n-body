//
// Created by Jackson Campolattaro on 1/26/23.
//

#ifndef N_BODY_DUALTRAVERSALSOLVER_H
#define N_BODY_DUALTRAVERSALSOLVER_H

#include <NBody/Simulation/Solvers/DualTraversalSolverBase.h>

namespace NBody {

    template<typename DualTree, typename DescentCriterionType>
    class DualTraversalSolver : public DualTraversalSolverBase<DualTree, DualTree, DescentCriterionType> {
    private:

        DualTree _tree;

        // todo: eliminate this, if possible
        using DualTraversalSolverBase<DualTree, DualTree, DescentCriterionType>::computeAccelerations;
        using DualTraversalSolverBase<DualTree, DualTree, DescentCriterionType>::collapseAccelerations;
        using DualTraversalSolverBase<DualTree, DualTree, DescentCriterionType>::_simulation;
        using DualTraversalSolverBase<DualTree, DualTree, DescentCriterionType>::_dt;
        using DualTraversalSolverBase<DualTree, DualTree, DescentCriterionType>::_statusDispatcher;

    public:

        DualTraversalSolver(Simulation &simulation, Physics::Rule &rule) :
                DualTraversalSolverBase<DualTree, DualTree, DescentCriterionType>(simulation, rule),
                _tree(simulation) {}

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
                    computeAccelerations(
                            _simulation.template view<const Position, Acceleration>(),
                            _simulation.template view<const Position, const Mass>(),
                            _tree.root(),
                            node
                    );
                });
            }

            {
                _statusDispatcher.emit({"Collapsing accelerations"});
                auto view = _simulation.template view<const Position, Acceleration>();
                // todo: find a better way of avoiding ambiguity
                collapseAccelerations(_tree.root(), view, typename DualTree::Node::Summary::Acceleration{});
            }
        }

    };
}

#endif //N_BODY_DUALTRAVERSALSOLVER_H
