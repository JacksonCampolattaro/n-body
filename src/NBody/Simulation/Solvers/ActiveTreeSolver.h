//
// Created by Jackson Campolattaro on 2/10/23.
//

#ifndef N_BODY_ACTIVETREESOLVER_H
#define N_BODY_ACTIVETREESOLVER_H

#include <NBody/Simulation/Solver.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/activeTree.h>

namespace NBody {

    using Descent::DescentCriterionType;

    template<typename TreeType, DescentCriterionType DescentCriterion>
    class ActiveTreeSolver : public Solver {
    private:

        TreeType _tree;
        DescentCriterion _descentCriterion{0.4f};

    public:

        ActiveTreeSolver(Simulation &simulation, Physics::Gravity &rule) :
                Solver(simulation, rule),
                _tree(simulation) {}

        void updateAccelerations() override {

            {
                // Construct an octree from the actor particles
                _statusDispatcher.emit({"Building Tree"});
                _tree.refine();
            }

            {
                _statusDispatcher.emit({"Resetting accelerations"});
                auto view = _simulation.view<Acceleration>();
                view.each([](Acceleration &acceleration) { acceleration = {0.0f, 0.0f, 0.0f}; });
            }

            {
                // Use the octree to estimate forces on each passive particle
                _statusDispatcher.emit({"Computing Accelerations"});
                auto view = _simulation.template view<const Position, Acceleration>();
                tbb::parallel_for_each(view, [&](Entity e) {
                    const auto &targetPosition = view.template get<const Position>(e);
                    auto &acceleration = view.template get<Acceleration>(e);

                    acceleration = Descent::activeTree(
                            _tree.root(), targetPosition,
                            _descentCriterion, _rule,
                            simulation().template view<const Position, const Mass>()
                    );
                });
            }
        }

        TreeType &tree() { return _tree; }

        const TreeType &tree() const { return _tree; }

        float &theta() { return _descentCriterion.theta(); }

        const float &theta() const { return _descentCriterion.theta(); }

    };

}

#endif //N_BODY_ACTIVETREESOLVER_H
