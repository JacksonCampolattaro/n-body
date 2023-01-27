//
// Created by Jackson Campolattaro on 1/21/23.
//

#ifndef N_BODY_COMPARISON_H
#define N_BODY_COMPARISON_H

#include <NBody/Simulation/Simulation.h>

#include <glm/gtx/norm.hpp>

namespace Comparison {

    using namespace NBody;
    using namespace Physics;

    using Metric = std::function<float(const Simulation &, Entity, const Simulation &, Entity)>;

    float L2Norm(const Simulation &A, Entity a, const Simulation &B, Entity b) {
        assert(A.get<Mass>(a).mass() == B.get<Mass>(b).mass());
        return glm::distance2(
                (glm::vec3) A.get<Acceleration>(a),
                (glm::vec3) B.get<Acceleration>(b)
        ) * A.get<Mass>(a).mass();
    }

    float L1Norm(const Simulation &A, Entity a, const Simulation &B, Entity b) {
        assert(A.get<Mass>(a).mass() == B.get<Mass>(b).mass());
        return glm::distance(
                (glm::vec3) A.get<Acceleration>(a),
                (glm::vec3) B.get<Acceleration>(b)
        ) * A.get<Mass>(a).mass();
    }

    using Comparator = std::function<float(const Metric &, const Simulation &, const Simulation &)>;

    float average(const Metric &metric, const Simulation &A, const Simulation &B) {
        auto AValues = A.view<const Velocity, const Mass>();
        auto BValues = B.view<const Velocity, const Mass>();
        // todo: ensure that aValues contains the same entities in the same order as bValues
        return std::transform_reduce(AValues.begin(), AValues.end(), BValues.begin(), 0.0f,
                                     std::plus{},
                                     [&](const auto &a, const auto &b) {
                                         return metric(A, a, B, b);
                                     }
        ) / (float) std::distance(AValues.begin(), BValues.end());
    }

    float maximum(const Metric &metric, const Simulation &A, const Simulation &B) {
        auto AValues = A.view<const Velocity, const Mass>();
        auto BValues = B.view<const Velocity, const Mass>();
        // todo: ensure that aValues contains the same entities in the same order as bValues
        return std::transform_reduce(AValues.begin(), AValues.end(), BValues.begin(), 0.0f,
                                     [&](float m1, float m2) {
                                         return std::max(m1, m2);
                                     },
                                     [&](const auto &a, const auto &b) {
                                         return metric(A, a, B, b);
                                     }
        );
    }

    float fractional(const Comparator &comparator, const Metric &metric,
                     const Simulation &reference, const Simulation &A, const Simulation &B) {
        return comparator(metric, B, reference) / comparator(metric, A, reference);
    }
}

#endif //N_BODY_COMPARISON_H
