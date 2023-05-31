//
// Created by Jackson Campolattaro on 2/23/23.
//

#ifndef N_BODY_MEANGRADER_H
#define N_BODY_MEANGRADER_H

#include "Grader.h"

namespace NBody {

    class MeanGrader : public Grader {
    public:

        explicit MeanGrader(json scenario, Physics::Gravity rule = Physics::Gravity{}) :
                Grader(scenario, rule) {}

        float error(const Simulation &candidateSimulation) const override {
            return averageError(candidateSimulation) * 100.0f;
        }

        float error(const Simulation &A, Entity a, const Simulation &B, Entity b) const override {
            assert(a == b);
            assert(A.get<Physics::Mass>(a).mass() == B.get<Physics::Mass>(b).mass());

            auto forceA = A.get<Physics::Acceleration>(a) * A.get<Physics::Mass>(a).mass();
            auto forceB = B.get<Physics::Acceleration>(b) * B.get<Physics::Mass>(b).mass();
            float differenceInForces = glm::distance(forceA, forceB);

            if (glm::any(glm::isinf((glm::vec3)forceB) || glm::isnan((glm::vec3)forceB)))
                return std::numeric_limits<float>::infinity();

            return differenceInForces / glm::length(forceA);
        }

    };

}

#endif //N_BODY_MEANGRADER_H
