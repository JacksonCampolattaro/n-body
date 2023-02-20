//
// Created by Jackson Campolattaro on 2/20/23.
//

#ifndef N_BODY_RMSGRADER_H
#define N_BODY_RMSGRADER_H

#include "Grader.h"

namespace NBody {

    class RMSGrader : public Grader {
    public:

        explicit RMSGrader(json scenario, Physics::Rule rule = Physics::Rule{}) :
                Grader(scenario, rule) {}

        float error(const Simulation &candidateSimulation) const override {
            return std::sqrt(maximumError(candidateSimulation)) * 100.0f;
        }

        float error(const Simulation &A, Entity a, const Simulation &B, Entity b) const override {
            assert(a == b);
            assert(A.get<Physics::Mass>(a).mass() == B.get<Physics::Mass>(b).mass());

            auto forceA = A.get<Physics::Acceleration>(a) * A.get<Physics::Mass>(a).mass();
            auto forceB = B.get<Physics::Acceleration>(b) * B.get<Physics::Mass>(b).mass();
            float differenceInForces = glm::distance(forceA, forceB);

            return std::pow(differenceInForces / glm::length(forceA), 2.0f);
        }

    };

}

#endif //N_BODY_RMSGRADER_H
