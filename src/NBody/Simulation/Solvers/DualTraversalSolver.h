//
// Created by Jackson Campolattaro on 1/26/23.
//

#ifndef N_BODY_DUALTRAVERSALSOLVER_H
#define N_BODY_DUALTRAVERSALSOLVER_H

#include <NBody/Simulation/Solver.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/balancedLockstepDualTree.h>
#include <NBody/Simulation/Solvers/Descent/collapseAccelerations.h>

namespace NBody {

    using Descent::DescentCriterionType;

    template<typename DualTree, DescentCriterionType DescentCriterion, RuleType Rule = Gravity>
    class DualTraversalSolver : public Solver<Rule> {
    private:

        DualTree _tree;

        DescentCriterion _descentCriterion{0.4};

    public:

        DualTraversalSolver(Simulation &simulation, Rule &rule) :
                Solver<Rule>(simulation, rule),
                _tree(simulation) {}

        DescentCriterion &descentCriterion() { return _descentCriterion; }

        const DescentCriterion &descentCriterion() const { return _descentCriterion; }

        float &theta() { return _descentCriterion.theta(); }

        [[nodiscard]] const float &theta() const { return _descentCriterion.theta(); }

        const DualTree &tree() const { return _tree; }

        DualTree &tree() { return _tree; }

        void updateAccelerations() override {

            {
                this->_statusDispatcher.emit({"Building dual tree"});
                _tree.refine();
            }

            {
                this->_statusDispatcher.emit({"Resetting accelerations"});
                auto view = this->_simulation.template view<Acceleration>();
                view.each([](Acceleration &acceleration) { acceleration = {0.0f, 0.0f, 0.0f}; });
            }

            {
                this->_statusDispatcher.emit({"Computing accelerations"});
                auto startingNodes = _tree.loadBalancedBreak(64);
                tbb::parallel_for_each(startingNodes, [&](std::reference_wrapper<typename DualTree::Node> passiveNode) {
                    Descent::balancedLockstepDualTree(
                            _tree.root(), passiveNode.get(),
                            _descentCriterion, this->_rule,
                            this->_simulation.template view<const Position, const Mass>(),
                            this->_simulation.template view<const Position, Acceleration>()
                    );
                });
            }

            {
                this->_statusDispatcher.emit({"Collapsing accelerations"});
                auto view = this->_simulation.template view<const Position, Acceleration>();
                Descent::collapseAccelerations(_tree.root(), view);
            }
        }

    };
}

#endif //N_BODY_DUALTRAVERSALSOLVER_H
