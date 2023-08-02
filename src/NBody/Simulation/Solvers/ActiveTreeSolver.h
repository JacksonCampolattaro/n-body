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

    template<typename TreeType, DescentCriterionType DescentCriterion, RuleType Rule>
    class ActiveTreeSolver : public Solver<Rule> {
    private:

        TreeType _tree;
        DescentCriterion _descentCriterion{0.3f};

    public:

        ActiveTreeSolver(Simulation &simulation, Rule &rule) :
                Solver<Rule>(simulation, rule),
                _tree(simulation) {}

        DescentCriterion &descentCriterion() { return _descentCriterion; }

        const DescentCriterion &descentCriterion() const { return _descentCriterion; }

        void updateAccelerations() override {

            {
                // Construct an octree from the actor particles
                this->_statusDispatcher.emit({"Building Tree"});
                _tree.refine();
                //spdlog::debug("Max tree depth = {}", _tree.root().depth());
            }

            {
                this->_statusDispatcher.emit({"Resetting accelerations"});
                auto view = this->_simulation.template view<Acceleration>();
                view.each([](Acceleration &acceleration) { acceleration = {0.0f, 0.0f, 0.0f}; });
            }

            {
                // Use the octree to estimate forces on each passive particle
                this->_statusDispatcher.emit({"Computing Accelerations"});
                auto view = this->_simulation.template view<const Position, Acceleration>();

                //                view.each([&](const Position &targetPosition, Acceleration &acceleration) {
                //                    acceleration = Descent::pipelineActiveTree(
                //                            _tree.root(), targetPosition,
                //                            _descentCriterion, this->_rule,
                //                            this->simulation().template view<const Position, const Mass>()
                //                    );
                //                });

                tbb::parallel_for_each(view, [&](Entity e) {
                    const auto &targetPosition = view.template get<const Position>(e);
                    auto &acceleration = view.template get<Acceleration>(e);

                    acceleration = Descent::activeTree(
                            _tree.root(), targetPosition,
                            _descentCriterion, this->_rule,
                            this->simulation().template view<const Position, const Mass>()
                    );
                });
            }
        }

        TreeType &tree() { return _tree; }

        [[nodiscard]] const TreeType &tree() const { return _tree; }

        float &theta() { return _descentCriterion.theta(); }

        [[nodiscard]] const float &theta() const { return _descentCriterion.theta(); }

    };

    //template<typename TreeType, DescentCriterionType DescentCriterion, RuleType Rule, RuleType NewRule>

}

#endif //N_BODY_ACTIVETREESOLVER_H
