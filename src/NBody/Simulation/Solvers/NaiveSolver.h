//
// Created by Jackson Campolattaro on 19/04/2022.
//

#ifndef N_BODY_NAIVESOLVER_H
#define N_BODY_NAIVESOLVER_H

#include <NBody/Simulation/Solver.h>

namespace NBody {

    template<RuleType Rule = Gravity>
    class NaiveSolver : public Solver<Rule> {
    public:

        using Solver<Rule>::Solver;

        std::string id() override { return "naive"; }

        std::string name() override { return "Naive"; }

        void updateAccelerations() override {

            {
                this->_statusDispatcher.emit({"Resetting accelerations"});
                auto view = this->_simulation.template view<Acceleration>();
                view.each([](Acceleration &acceleration) { acceleration = {0.0f, 0.0f, 0.0f}; });
            }

            // Compute accelerations
            {
                this->_statusDispatcher.emit({"Computing accelerations"});
                auto view = this->_simulation.template view<const Position, Acceleration>();
                auto actorsView = this->_simulation.template view<const Position, const Mass>();
                tbb::parallel_for_each(view, [&](Entity e) {
                    const auto &passivePosition = view.template get<const Position>(e);
                    auto &passiveAcceleration = view.template get<Acceleration>(e);

                    actorsView.each([&](const Position &activePosition, const Mass &activeMass) {
                        passiveAcceleration += (glm::vec3) this->_rule(activePosition, activeMass, passivePosition);
                    });
                });
            }
        }
    };

}


#endif //N_BODY_NAIVESOLVER_H
