//
// Created by Jackson Campolattaro on 2/20/23.
//

#ifndef N_BODY_RMSGRADER_H
#define N_BODY_RMSGRADER_H

#include "Grader.h"

namespace NBody {

    class RMSGrader : public NaiveReferenceGrader {
    public:

        using NaiveReferenceGrader::NaiveReferenceGrader;

        float error(const Simulation &candidateSimulation) const override {
            return std::sqrt(maximumError(candidateSimulation)) * 100.0f;
        }

        bool acceptable(const Simulation &candidateSimulation) const override {
            return error(candidateSimulation) < 1.0;
        };

        float error(const Simulation &A, Entity a, const Simulation &B, Entity b) const override {
            assert(a == b);
            assert(A.get<Physics::Mass>(a).mass() == B.get<Physics::Mass>(b).mass());

            auto forceA = A.get<Physics::Acceleration>(a) * A.get<Physics::Mass>(a).mass();
            auto forceB = B.get<Physics::Acceleration>(b) * B.get<Physics::Mass>(b).mass();
            float differenceInForces = glm::distance(forceA, forceB);

            if (glm::any(glm::isinf((glm::vec3) forceB) || glm::isnan((glm::vec3) forceB)))
                return std::numeric_limits<float>::infinity();

            return std::pow(differenceInForces / glm::length(forceA), 2.0f);
        }

    };

}

#endif //N_BODY_RMSGRADER_H
