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

        explicit ConstitutionalGrader(json scenario, Physics::Gravity rule = Physics::Gravity{}) :
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

            if (glm::any(glm::isinf((glm::vec3)forceB) || glm::isnan((glm::vec3)forceB)))
                return std::numeric_limits<float>::infinity();

            return 100.0f * (differenceInForces / std::min(glm::length(forceA), _rmsForce));
        }

    };

}

#endif //N_BODY_CONSTITUTIONALGRADER_H
