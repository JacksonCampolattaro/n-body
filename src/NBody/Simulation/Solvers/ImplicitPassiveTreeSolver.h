//
// Created by Jackson Campolattaro on 5/3/23.
//

#ifndef N_BODY_IMPLICITPASSIVETREESOLVER_H
#define N_BODY_IMPLICITPASSIVETREESOLVER_H

#include <NBody/Simulation/Solver.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/passiveTreeImplicitField.h>
#include <NBody/Simulation/Solvers/Descent/collapseAccelerations.h>

namespace NBody {

    using Descent::DescentCriterionType;

    template<typename TreeType, DescentCriterionType DescentCriterion>
    class ImplicitPassiveTreeSolver : public Solver {
    private:

        TreeType _tree;
        DescentCriterion _descentCriterion{0.4f};

    public:

        ImplicitPassiveTreeSolver(Simulation &simulation, Physics::Gravity &rule) :
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

                auto startingNodes = _tree.loadBalancedBreak(256);
                auto view = _simulation.template view<const Position, const Mass>();
                tbb::parallel_for_each(startingNodes, [&](std::reference_wrapper<typename TreeType::Node> node) {
                    std::vector<Entity> activeEntities{view.begin(), view.end()};
                    std::span<Entity> activeEntitiesView{activeEntities};
                    Descent::passiveTreeImplicitField(
                            activeEntitiesView, view,
                            node.get(), _simulation.template view<const Position, Acceleration>(),
                            _descentCriterion, _rule
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


#endif //N_BODY_IMPLICITPASSIVETREESOLVER_H
