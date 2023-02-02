//
// Created by Jackson Campolattaro on 2/2/23.
//

#ifndef N_BODY_CONSTITUTIONALGRADER_H
#define N_BODY_CONSTITUTIONALGRADER_H

#include "Grader.h"

namespace NBody {

    class ConstitutionalGrader : public Grader {
    private:

        float _averageForce = 0.0f;

    public:

        explicit ConstitutionalGrader(json scenario, Physics::Rule rule = Physics::Rule{}) :
                Grader(scenario, rule) {

            // Determine the average force, to be used in scoring
            auto referenceValues = _referenceSimulation.view<const Physics::Acceleration, const Physics::Mass>();
            _averageForce = std::transform_reduce(referenceValues.begin(), referenceValues.end(), 0.0f, std::plus<>(),
                                                  [&](const auto &e) {
                                                      return glm::length(
                                                              referenceValues.get<const Physics::Acceleration>(e) *
                                                              referenceValues.get<const Physics::Mass>(e).mass()
                                                      );
                                                  }
            ) / (float) scenario["particles"].size();

            spdlog::info("Average force = {}", _averageForce);
        }

        const json &scenario() const { return _scenario; }

        Physics::Rule &rule() { return _rule; }

        float error(const Simulation &A, Entity a, const Simulation &B, Entity b) const override {
            assert(a == b);
            assert(A.get<Mass>(a).mass() == B.get<Mass>(b).mass());

            auto forceA = A.get<Physics::Acceleration>(a) * A.get<Physics::Mass>(a).mass();
            auto forceB = B.get<Physics::Acceleration>(b) * B.get<Physics::Mass>(b).mass();
            float differenceInForces = glm::distance(forceA, forceB);

            return 100.0f * (differenceInForces / std::min(glm::length(forceA), _averageForce));
        }

    };

}

#endif //N_BODY_CONSTITUTIONALGRADER_H
