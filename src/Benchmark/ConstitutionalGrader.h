//
// Created by Jackson Campolattaro on 2/2/23.
//

#ifndef N_BODY_CONSTITUTIONALGRADER_H
#define N_BODY_CONSTITUTIONALGRADER_H

#include "Grader.h"

namespace NBody {

    class ConstitutionalGrader : public Grader {
    private:

        float _rmsForce = 0.0f;

    public:

        explicit ConstitutionalGrader(json scenario, Physics::Rule rule = Physics::Rule{}) :
                Grader(scenario, rule) {

            // Determine the average force, to be used in scoring
            auto referenceValues = _referenceSimulation.view<const Physics::Acceleration, const Physics::Mass>();
            _rmsForce = std::sqrt(
                    std::transform_reduce(referenceValues.begin(), referenceValues.end(), 0.0f, std::plus<>(),
                                          [&](const auto &e) {
                                              return glm::length2(
                                                      referenceValues.get<const Physics::Acceleration>(e) *
                                                      referenceValues.get<const Physics::Mass>(e).mass()
                                              );
                                          }
                    ) / (float) scenario["particles"].size());
        }

        float error(const Simulation &candidateSimulation) const override { return maximumError(candidateSimulation); }

        float error(const Simulation &A, Entity a, const Simulation &B, Entity b) const override {
            assert(a == b);
            assert(A.get<Physics::Mass>(a).mass() == B.get<Physics::Mass>(b).mass());

            auto forceA = A.get<Physics::Acceleration>(a) * A.get<Physics::Mass>(a).mass();
            auto forceB = B.get<Physics::Acceleration>(b) * B.get<Physics::Mass>(b).mass();
            float differenceInForces = glm::distance(forceA, forceB);

            auto error = 100.0f * (differenceInForces / std::min(glm::length(forceA), _rmsForce));
            if (error > 67.0f) spdlog::error((ENTT_ID_TYPE) b);
            return error;
        }

    };

}

#endif //N_BODY_CONSTITUTIONALGRADER_H
