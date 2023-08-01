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
    protected:

        ActiveTree _activeTree;
        PassiveTree _passiveTree;

        DescentCriterion _descentCriterion{0.7f};

    public:

        ImplicitDualTreeSolver(Simulation &simulation, Rule &rule) :
                Solver<Rule>(simulation, rule),
                _activeTree(simulation),
                _passiveTree(simulation) {
            _passiveTree.maxLeafSize() = 128;
        }

        DescentCriterion &descentCriterion() { return _descentCriterion; }

        const DescentCriterion &descentCriterion() const { return _descentCriterion; }

        float &theta() { return _descentCriterion.theta(); }

        [[nodiscard]] const float &theta() const { return _descentCriterion.theta(); }

        [[nodiscard]] const ActiveTree &activeTree() const { return _activeTree; }

        ActiveTree &activeTree() { return _activeTree; }

        [[nodiscard]] const PassiveTree &passiveTree() const { return _passiveTree; }

        PassiveTree &passiveTree() { return _passiveTree; }

        void updateAccelerations() override {

            tbb::task_group treeBuildingTaskGroup;
            treeBuildingTaskGroup.run([&]() {
                this->_statusDispatcher.emit({"Building active tree"});
                _activeTree.refine();
                spdlog::trace("Active Tree has a max depth of {}", activeTree().root().depth());
            });
            treeBuildingTaskGroup.run([&]() {
                this->_statusDispatcher.emit({"Building passive tree"});
                _passiveTree.refine();
                spdlog::trace("Passive Tree has a max depth of {}", passiveTree().root().depth());
            });
            treeBuildingTaskGroup.wait();

            {
                this->_statusDispatcher.emit({"Resetting accelerations"});
                auto view = this->_simulation.template view<Acceleration>();
                view.each([](Acceleration &acceleration) { acceleration = {0.0f, 0.0f, 0.0f}; });
            }

            {
                this->_statusDispatcher.emit({"Computing accelerations"});
                auto startingNodes = _passiveTree.loadBalancedBreak(256);
                tbb::parallel_for_each(startingNodes, [&](std::reference_wrapper<typename PassiveTree::Node> node) {
                    Descent::balancedLockstepDualTreeImplicitField<typename ActiveTree::Node>(
                            _activeTree.root(), node.get(),
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
