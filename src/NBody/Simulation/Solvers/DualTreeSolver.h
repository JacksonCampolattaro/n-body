//
// Created by Jackson Campolattaro on 1/16/23.
//

#ifndef N_BODY_DUALTREESOLVER_H
#define N_BODY_DUALTREESOLVER_H

#include "Trees/LinearBVH.h"
#include "Trees/Octree.h"
#include "Trees/DescentCriterion.h"
#include <tbb/parallel_for_each.h>

#include <span>
#include <memory>

#include <NBody/Simulation/Solvers/DualTraversalSolverBase.h>


namespace NBody {

    template<typename ActiveTree, typename PassiveTree, typename DescentCriterionType>
    class DualTreeSolver : public DualTraversalSolverBase<ActiveTree, PassiveTree, DescentCriterionType> {
    private:

        ActiveTree _activeTree;
        PassiveTree _passiveTree;

        // todo: eliminate this, if possible
        using DualTraversalSolverBase<ActiveTree, PassiveTree, DescentCriterionType>::computeAccelerations;
        using DualTraversalSolverBase<ActiveTree, PassiveTree, DescentCriterionType>::collapseAccelerations;
        using DualTraversalSolverBase<ActiveTree, PassiveTree, DescentCriterionType>::_simulation;
        using DualTraversalSolverBase<ActiveTree, PassiveTree, DescentCriterionType>::_dt;
        using DualTraversalSolverBase<ActiveTree, PassiveTree, DescentCriterionType>::_statusDispatcher;

    public:

        DualTreeSolver(Simulation &simulation, Physics::Rule &rule) :
                DualTraversalSolverBase<ActiveTree, PassiveTree, DescentCriterionType>(simulation, rule),
                _activeTree(simulation),
                _passiveTree(simulation) {}

        const ActiveTree &activeTree() const { return _activeTree; }

        ActiveTree &activeTree() { return _activeTree; }

        const PassiveTree &passiveTree() const { return _passiveTree; }

        PassiveTree &passiveTree() { return _passiveTree; }

        void step() override {

            _statusDispatcher.emit({"Building active tree"});
            _activeTree.refine();

            _statusDispatcher.emit({"Building passive tree"});
            _passiveTree.refine();

            {
                _statusDispatcher.emit({"Resetting accelerations"});
                auto view = _simulation.template view<Acceleration>();
                view.each([](Acceleration &acceleration) { acceleration = {0.0f, 0.0f, 0.0f}; });
            }

            {
                _statusDispatcher.emit({"Computing accelerations"});
                auto startingNodes = _passiveTree.loadBalancedBreak(256);
                tbb::parallel_for_each(startingNodes, [&](std::reference_wrapper<typename PassiveTree::Node> node) {
                    computeAccelerations(
                            _simulation.template view<const Position, Acceleration>(),
                            _simulation.template view<const Position, const Mass>(),
                            _activeTree.root(),
                            node
                    );
                });
            }

            {
                _statusDispatcher.emit({"Collapsing accelerations"});
                auto view = _simulation.template view<Acceleration>();
                collapseAccelerations(_passiveTree.root(), view);
            }

            // Update velocities, based on acceleration
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

}

#endif //N_BODY_DUALTREESOLVER_H
