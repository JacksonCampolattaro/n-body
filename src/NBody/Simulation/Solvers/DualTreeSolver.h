//
// Created by Jackson Campolattaro on 1/16/23.
//

#ifndef N_BODY_DUALTREESOLVER_H
#define N_BODY_DUALTREESOLVER_H

#include <NBody/Simulation/Solver.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/collapseAccelerations.h>
#include <NBody/Simulation/Solvers/Descent/lockstepDualTree.h>
#include <NBody/Simulation/Solvers/Descent/adaptiveDualTree.h>

namespace NBody {

    using Descent::DescentCriterionType;

    template<typename ActiveTree, typename PassiveTree, DescentCriterionType DescentCriterion>
    class DualTreeSolver : public Solver {
    private:

        ActiveTree _activeTree;
        PassiveTree _passiveTree;

        DescentCriterion _descentCriterion{0.4f};

    public:

        DualTreeSolver(Simulation &simulation, Physics::Rule &rule) :
                Solver(simulation, rule),
                _activeTree(simulation),
                _passiveTree(simulation) {}

        float &theta() { return _descentCriterion.theta(); }

        const float &theta() const { return _descentCriterion.theta(); }

        const ActiveTree &activeTree() const { return _activeTree; }

        ActiveTree &activeTree() { return _activeTree; }

        const PassiveTree &passiveTree() const { return _passiveTree; }

        PassiveTree &passiveTree() { return _passiveTree; }

        void updateAccelerations() override {

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
                    Descent::adaptiveDualTree(
                            _activeTree.root(), node.get(),
                            _descentCriterion, _rule,
                            _simulation.template view<const Position, const Mass>(),
                            _simulation.template view<const Position, Acceleration>()
                    );
                });
            }

            {
                _statusDispatcher.emit({"Collapsing accelerations"});
                auto view = _simulation.template view<const Position, Acceleration>();
                Descent::collapseAccelerations(_passiveTree.root(), view);
            }
        }

    };

}

#endif //N_BODY_DUALTREESOLVER_H
