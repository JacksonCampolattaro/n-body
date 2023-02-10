//
// Created by Jackson Campolattaro on 1/26/23.
//

#ifndef N_BODY_DUALTRAVERSALSOLVER_H
#define N_BODY_DUALTRAVERSALSOLVER_H


#include "Trees/Octree.h"
#include "Trees/DescentCriterion.h"
#include <tbb/parallel_for_each.h>

#include <span>
#include <memory>

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

        void step() override {

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
                auto view = _simulation.template view<Acceleration>();
                collapseAccelerations(_tree.root(), view);
            }

            // Update velocities, based on force
            {
                _statusDispatcher.emit({"Updating velocities"});
                auto view = _simulation.template view<const Acceleration, Velocity>();
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
                auto view = _simulation.template view<const Velocity, Position>();

                tbb::parallel_for_each(view, [&](Entity e) {
                    const auto &v = view.template get<const Velocity>(e);
                    auto &p = view.template get<Position>(e);
                    p = p + (v * _dt);
                });
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
