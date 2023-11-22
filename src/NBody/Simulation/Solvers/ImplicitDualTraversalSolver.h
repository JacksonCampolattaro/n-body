//
// Created by Jackson Campolattaro on 5/5/23.
//

#ifndef N_BODY_IMPLICITDUALTRAVERSALSOLVER_H
#define N_BODY_IMPLICITDUALTRAVERSALSOLVER_H

#include <NBody/Simulation/Solver.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/balancedLockstepDualTreeImplicitField.h>

namespace NBody {

    using Descent::DescentCriterionType;

    template<typename DualTree, DescentCriterionType DescentCriterion, RuleType Rule>
    class ImplicitDualTraversalSolver : public Solver<Rule> {
    private:

        DualTree _tree;

        DescentCriterion _descentCriterion{0.4};

    public:

        ImplicitDualTraversalSolver(Simulation &simulation, Rule &rule) :
                Solver<Rule>(simulation, rule),
                _tree(simulation) {}

        DescentCriterion &descentCriterion() { return _descentCriterion; }

        const DescentCriterion &descentCriterion() const { return _descentCriterion; }

        float &theta() { return _descentCriterion.theta(); }

        [[nodiscard]] const float &theta() const { return _descentCriterion.theta(); }

        [[nodiscard]] const DualTree &tree() const { return _tree; }

        DualTree &tree() { return _tree; }

        Acceleration sampleAcceleration(const Position &position) override {
            // todo: actually use the dual-tree algorithm!

            // It's okay if the tree is a little bit out of date, as long as it's been built
            if (_tree.root().isLeaf())
                _tree.refine();

            return Descent::activeTree(
                    _tree.root(), position,
                    _descentCriterion, this->_rule,
                    this->_simulation.template view<const Position, const Mass>()
            );
        }

        void updateAccelerations() override {

            {
                this->_statusDispatcher.emit({"Building dual tree"});
                _tree.refine();
                //spdlog::debug("Tree has a max depth of {}", tree().root().depth());
            }

            {
                this->_statusDispatcher.emit({"Resetting accelerations"});
                auto view = this->_simulation.template view<Acceleration>();
                view.each([](Acceleration &acceleration) { acceleration = {0.0f, 0.0f, 0.0f}; });
            }

            {
                this->_statusDispatcher.emit({"Computing accelerations"});
                //auto startingNodes = _tree.depthBreak(8);
                auto startingNodes = _tree.loadBalancedBreak(256);
                tbb::parallel_for_each(startingNodes, [&](std::reference_wrapper<typename DualTree::Node> passiveNode) {
                    Descent::balancedLockstepDualTreeImplicitField(
                            _tree.root(), passiveNode.get(),
                            _descentCriterion, this->_rule,
                            this->_simulation.template view<const Position, const Mass>(),
                            this->_simulation.template view<const Position, Acceleration>()
                    );
                });
            }
        }

    };
}

#endif //N_BODY_IMPLICITDUALTRAVERSALSOLVER_H
