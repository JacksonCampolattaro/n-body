//
// Created by Jackson Campolattaro on 5/4/23.
//

#ifndef N_BODY_IMPLICITDUALTREESOLVER_H
#define N_BODY_IMPLICITDUALTREESOLVER_H

#include <NBody/Simulation/Solver.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/adaptiveDualTreeImplicitField.h>
#include <NBody/Simulation/Solvers/Descent/balancedLockstepDualTreeImplicitField.h>

namespace NBody {

    using Descent::DescentCriterionType;

    template<typename ActiveTree, typename PassiveTree, DescentCriterionType DescentCriterion, RuleType Rule = Gravity>
    class ImplicitDualTreeSolver : public Solver<Rule> {
    private:

        ActiveTree _activeTree;
        PassiveTree _passiveTree;

        DescentCriterion _descentCriterion{0.4f};

    public:

        ImplicitDualTreeSolver(Simulation &simulation, Rule &rule) :
                Solver<Rule>(simulation, rule),
                _activeTree(simulation),
                _passiveTree(simulation) {}

        float &theta() { return _descentCriterion.theta(); }

        [[nodiscard]] const float &theta() const { return _descentCriterion.theta(); }

        [[nodiscard]] const ActiveTree &activeTree() const { return _activeTree; }

        ActiveTree &activeTree() { return _activeTree; }

        [[nodiscard]] const PassiveTree &passiveTree() const { return _passiveTree; }

        PassiveTree &passiveTree() { return _passiveTree; }

        void updateAccelerations() override {

            this->_statusDispatcher.emit({"Building active tree"});
            _activeTree.refine();

            this->_statusDispatcher.emit({"Building passive tree"});
            _passiveTree.refine();

            {
                this->_statusDispatcher.emit({"Resetting accelerations"});
                auto view = this->_simulation.template view<Acceleration>();
                view.each([](Acceleration &acceleration) { acceleration = {0.0f, 0.0f, 0.0f}; });
            }

            {
                this->_statusDispatcher.emit({"Computing accelerations"});
                auto startingNodes = _passiveTree.loadBalancedBreak(256);
                tbb::parallel_for_each(startingNodes, [&](std::reference_wrapper<typename PassiveTree::Node> node) {
                    std::vector<std::reference_wrapper<const typename ActiveTree::Node>> activeNodes{
                            _activeTree.root()
                    };
                    Descent::adaptiveDualTreeImplicitField<typename ActiveTree::Node>(
                            activeNodes, node.get(),
                            _descentCriterion, this->_rule,
                            this->_simulation.template view<const Position, const Mass>(),
                            this->_simulation.template view<const Position, Acceleration>()
                    );
                });
            }

        }

    };

}

#endif //N_BODY_IMPLICITDUALTREESOLVER_H
