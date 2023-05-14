//
// Created by jackcamp on 4/12/22.
//

#ifndef N_BODY_GRAVITY_H
#define N_BODY_GRAVITY_H

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>

#include <spdlog/spdlog.h>

#include "NBody/Physics/Position.h"
#include "NBody/Physics/Acceleration.h"
#include "NBody/Physics/Force.h"
#include "NBody/Physics/Mass.h"
#include "NBody/Physics/Summaries/SummaryType.h"

#include "NBody/Physics/Summaries/CenterOfMassSummary.h"
#include "NBody/Physics/Summaries/MultipoleMassSummary.h"
#include "NBody/Physics/Summaries/MultipoleAccelerationSummary.h"
#include "NBody/Physics/Summaries/CenterOfMassSummary.h"
#include "NBody/Physics/Summaries/AccelerationSummary.h"
#include "NBody/Physics/Summaries/MultipoleAccelerationSummary.h"

#include <NBody/Physics/Rules/RuleType.h>

namespace {

    using namespace NBody;
    using
    enum Dimension;

    template<std::size_t P, typename T>
    inline T pow(T value) {
        if constexpr (P == 0)
            return T(1);
        else
            return value * pow<P - 1>(value);
    }

    template<std::size_t Order>
    inline auto D(const glm::vec3 &R, float r) {

        float f1 = -1.0f / pow<2>(r);
        float f2 = 3.0f / pow<3>(r);
        float f3 = -15.0f / pow<4>(r);
        float f4 = 105.0f / pow<5>(r);
        float f5 = -945.0f / pow<6>(r);

        float g1 = f1 / r;
        float g2 = f2 / pow<2>(r);
        float g3 = f3 / pow<3>(r);
        float g4 = f4 / pow<4>(r);
        float g5 = f5 / pow<5>(r);

        if constexpr (Order == 1) {

            return g1 * R;

        } else if constexpr (Order == 2) {

            return (g2 * SymmetricTensor3<2>::cartesianPower(R)) + (g1 * SymmetricTensor3<2>::identity());

        } else if constexpr (Order == 3) {

            auto A = g3 * SymmetricTensor3<3>::cartesianPower(R);
            auto B = SymmetricTensor3<3>{};
            B.get<X, Y, Y>() = R.x;
            B.get<X, Z, Z>() = R.x;
            B.get<Y, X, X>() = R.y;
            B.get<Y, Z, Z>() = R.y;
            B.get<Z, X, X>() = R.z;
            B.get<Z, Y, Y>() = R.z;
            B += 3 * SymmetricTensor3<3>::diagonal(R);
            B *= g2;
            return A + B;

        } else if constexpr (Order == 4) {

            auto A = g4 * SymmetricTensor3<4>::cartesianPower(R);

            // Why doesn't R get factored into this term?
            SymmetricTensor3<4> B{};
            B.get<X, X, Y, Y>() = 1;
            B.get<X, X, Z, Z>() = 1;
            B.get<Y, Y, Z, Z>() = 1;
            B += SymmetricTensor3<4>::diagonal({3, 3, 3});
            B *= g2;

            SymmetricTensor3<4> C{};
            SymmetricTensor3<2> R2 = SymmetricTensor3<2>::cartesianPower(R);
            C += 6.0f * SymmetricTensor3<4>::diagonal(R2.diagonal());
            C.get<X, X, X, Y>() = 3 * R2.get<X, Y>();
            C.get<X, Y, Y, Y>() = 3 * R2.get<X, Y>();
            C.get<X, X, X, Z>() = 3 * R2.get<X, Z>();
            C.get<X, Z, Z, Z>() = 3 * R2.get<X, Z>();
            C.get<Y, Y, Y, Z>() = 3 * R2.get<Y, Z>();
            C.get<Y, Z, Z, Z>() = 3 * R2.get<Y, Z>();
            C.get<X, X, Y, Y>() = R2.get<X, X>() + R2.get<Y, Y>();
            C.get<X, X, Z, Z>() = R2.get<X, X>() + R2.get<Z, Z>();
            C.get<Y, Y, Z, Z>() = R2.get<Y, Y>() + R2.get<Z, Z>();
            C.get<X, X, Y, Z>() = R2.get<Y, Z>();
            C.get<X, Y, Y, Z>() = R2.get<X, Z>();
            C.get<X, Y, Z, Z>() = R2.get<X, Y>();
            C *= g3;

            return A + B + C;

        } else if constexpr (Order == 5) {

            SymmetricTensor3<2> R2 = SymmetricTensor3<2>::cartesianPower(R);
            SymmetricTensor3<3> R3 = SymmetricTensor3<3>::cartesianPower(R);

            auto A = g5 * SymmetricTensor3<5>::cartesianPower(R);

            // ~~~

            SymmetricTensor3<5> B{};
            B.get<X, X, X, X, X>() += 15 * g3 * R.x;
            B.get<Y, Y, Y, Y, Y>() += 15 * g3 * R.y;
            B.get<Z, Z, Z, Z, Z>() += 15 * g3 * R.z;

            B.get<X, X, X, X, Y>() += 3 * g3 * R.y;
            B.get<X, X, X, X, Z>() += 3 * g3 * R.z;
            B.get<X, Y, Y, Y, Y>() += 3 * g3 * R.x;
            B.get<X, Z, Z, Z, Z>() += 3 * g3 * R.x;
            B.get<Y, Y, Y, Y, Z>() += 3 * g3 * R.z;
            B.get<Y, Z, Z, Z, Z>() += 3 * g3 * R.y;

            B.get<X, X, X, Y, Y>() += 3 * g3 * R.x;
            B.get<X, X, X, Z, Z>() += 3 * g3 * R.x;
            B.get<X, X, Y, Y, Y>() += 3 * g3 * R.y;
            B.get<X, X, Z, Z, Z>() += 3 * g3 * R.z;
            B.get<Y, Y, Y, Z, Z>() += 3 * g3 * R.y;
            B.get<Y, Y, Z, Z, Z>() += 3 * g3 * R.z;

            B.get<X, X, Y, Z, Z>() += g3 * R.y;
            B.get<X, X, Y, Y, Z>() += g3 * R.z;
            B.get<X, Y, Y, Z, Z>() += g3 * R.x;

            B.get<X, X, X, Y, Z>() += 0;
            B.get<X, Y, Y, Y, Z>() += 0;
            B.get<X, Y, Z, Z, Z>() += 0;

            // ~~~

            SymmetricTensor3<5> C{};
            C.get<X, X, X, X, X>() += 10 * g4 * R3.get<X, X, X>();
            C.get<Y, Y, Y, Y, Y>() += 10 * g4 * R3.get<Y, Y, Y>();
            C.get<Z, Z, Z, Z, Z>() += 10 * g4 * R3.get<Z, Z, Z>();

            C.get<X, X, X, X, Y>() += 6 * g4 * R3.get<X, X, Y>();
            C.get<X, X, X, X, Z>() += 6 * g4 * R3.get<X, X, Z>();
            C.get<X, Y, Y, Y, Y>() += 6 * g4 * R3.get<Y, Y, X>();
            C.get<X, Z, Z, Z, Z>() += 6 * g4 * R3.get<Z, Z, X>();
            C.get<Y, Y, Y, Y, Z>() += 6 * g4 * R3.get<Y, Y, Z>();
            C.get<Y, Z, Z, Z, Z>() += 6 * g4 * R3.get<Z, Z, Y>();

            C.get<X, X, X, Y, Y>() += g4 * R3.get<X, X, X>() + 3 * g4 * R3.get<X, Y, Y>();
            C.get<X, X, X, Z, Z>() += g4 * R3.get<X, X, X>() + 3 * g4 * R3.get<X, Z, Z>();
            C.get<X, X, Y, Y, Y>() += g4 * R3.get<Y, Y, Y>() + 3 * g4 * R3.get<Y, X, X>();
            C.get<X, X, Z, Z, Z>() += g4 * R3.get<Z, Z, Z>() + 3 * g4 * R3.get<Z, X, X>();
            C.get<Y, Y, Y, Z, Z>() += g4 * R3.get<Y, Y, Y>() + 3 * g4 * R3.get<Y, Z, Z>();
            C.get<Y, Y, Z, Z, Z>() += g4 * R3.get<Z, Z, Z>() + 3 * g4 * R3.get<Z, Y, Y>();

            C.get<X, X, Y, Z, Z>() += g4 * R3.get<Y, Z, Z>() + g4 * R3.get<X, X, Y>();
            C.get<X, X, Y, Y, Z>() += g4 * R3.get<Y, Y, Z>() + g4 * R3.get<X, X, Z>();
            C.get<X, Y, Y, Z, Z>() += g4 * R3.get<X, Z, Z>() + g4 * R3.get<X, Y, Y>();

            C.get<X, X, X, Y, Z>() += 3 * g4 * R3.get<X, Y, Z>();
            C.get<X, Y, Y, Y, Z>() += 3 * g4 * R3.get<X, Y, Z>();
            C.get<X, Y, Z, Z, Z>() += 3 * g4 * R3.get<X, Y, Z>();

            // todo: is this right?

            return A + B + C;
        }
    }
}

namespace NBody::Physics {

    class Gravity : public RuleBase<Gravity> {
    private:

        float _g;
        float _epsilon;

    public:

        using RuleBase<Gravity>::operator();

        explicit Gravity(const float &G = 1.0, const float &epsilon = 0.0001) : _g(G), _epsilon(epsilon) {};

    public: // Particle-particle interaction

        [[nodiscard]] Acceleration particleParticle(const Position &activePosition,
                                                    const Mass &activeMass,
                                                    const Position &passivePosition) const {

            if (activePosition == passivePosition) return {};

            // See: https://github.com/hannorein/rebound/blob/9fb9ee9aa20c547e1e6c67e7a58f07fd7176c181/src/gravity.c
            glm::vec3 R = passivePosition - activePosition;
            float r = std::sqrt(glm::length2(R) + _epsilon);
            return -R * activeMass.mass() * _g / pow<3>(r);
        }

    public: // Node-particle interaction

        [[nodiscard]] Acceleration nodeParticle(const Position &activePosition,
                                                const CenterOfMassSummary &activeSummary,
                                                const Position &passivePosition) const {

            // If the active node isn't multipolar, we can just treat it like a single particle
            return particleParticle(activePosition, activeSummary.totalMass(),
                                    passivePosition);
        }

        template<std::size_t Order>
        [[nodiscard]] Acceleration nodeParticle(const Position &activePosition,
                                                const MultipoleMassSummary<Order> &activeSummary,
                                                const Position &passivePosition) const {

            if (activePosition == passivePosition) return {};

            glm::vec3 R = passivePosition - activePosition;
            float r = std::sqrt(glm::length2(R) + _epsilon);

            glm::vec3 dPhi = R * -activeSummary.totalMass().mass() / pow<3>(r);
            if constexpr (Order >= 2)
                dPhi += D<3>(R, r) * activeSummary.moment().template tensor<2>() / 2.0f;
            if constexpr (Order >= 3)
                dPhi -= D<4>(R, r) * activeSummary.moment().template tensor<3>() / 6.0f;
            if constexpr (Order >= 4)
                dPhi += D<5>(R, r) * activeSummary.moment().template tensor<4>() / 24.0f;
            if constexpr (Order >= 5)
                dPhi -= D<6>(R, r) * activeSummary.moment().template tensor<5>() / 120.0f;

            return _g * dPhi;
        }

    public: // Particle-node interaction

        void particleNode(
                const Position &activePosition,
                const Mass &activeMass,
                const Position &passivePosition,
                AccelerationSummary &passiveSummary
        ) const {
            passiveSummary.acceleration() += particleParticle(activePosition, activeMass, passivePosition);
        }

        template<std::size_t PassiveOrder>
        void particleNode(
                const Position &activePosition,
                const Mass &activeMass,
                const Position &passivePosition,
                MultipoleAccelerationSummary<PassiveOrder> &passiveSummary
        ) const {

            if (activePosition == passivePosition) return;

            // See: https://github.com/weiguangcui/Gadget4/blob/7d3b425e3e0aa7b6b0e0dbefa1d4120c55980a8f/src/fmm/fmm.cc

            glm::vec3 R = passivePosition - activePosition;
            float r = std::sqrt(glm::length2(R) + _epsilon);

            passiveSummary.acceleration().vector() -= (R / pow<3>(r)) * activeMass.mass() * _g;
            if constexpr (PassiveOrder >= 2)
                passiveSummary.acceleration().template tensor<2>() += D<2>(R, r) * activeMass.mass() * _g;
            if constexpr (PassiveOrder >= 3)
                passiveSummary.acceleration().template tensor<3>() += D<3>(R, r) * activeMass.mass() * _g / 2.0f;
            if constexpr (PassiveOrder >= 4)
                passiveSummary.acceleration().template tensor<4>() += D<4>(R, r) * activeMass.mass() * _g / 6.0f;
            if constexpr (PassiveOrder >= 5)
                passiveSummary.acceleration().template tensor<5>() += D<5>(R, r) * activeMass.mass() * _g / 24.0f;
        }

    public: // Node-node interaction

        void nodeNode(
                const Position &activePosition,
                const CenterOfMassSummary &activeSummary,
                const Position &passivePosition,
                AccelerationSummary &passiveSummary
        ) const {
            passiveSummary.acceleration() += particleParticle(activePosition, activeSummary.totalMass(),
                                                              passivePosition);
        }

        template<std::size_t Order>
        void nodeNode(
                const Position &activePosition,
                const CenterOfMassSummary &activeSummary,
                const Position &passivePosition,
                MultipoleAccelerationSummary<Order> &passiveSummary
        ) const {
            particleNode(activePosition, activeSummary.totalMass(),
                         passivePosition, passiveSummary);
        }


        template<std::size_t ActiveOrder, std::size_t PassiveOrder>
        void nodeNode(
                const Position &activePosition,
                const MultipoleMassSummary<ActiveOrder> &activeSummary,
                const Position &passivePosition,
                MultipoleAccelerationSummary<PassiveOrder> &passiveSummary
        ) const {

            // todo: temporary constraint, for simplicity & consistency with GADGET-4
            static_assert(ActiveOrder == PassiveOrder - 1);

            if (activePosition == passivePosition) return;

            // See: https://github.com/weiguangcui/Gadget4/blob/7d3b425e3e0aa7b6b0e0dbefa1d4120c55980a8f/src/fmm/fmm.cc

            glm::vec3 R = passivePosition - activePosition;
            float r = std::sqrt(glm::length2(R) + _epsilon);
            const auto &activeMass = activeSummary.totalMass();

            passiveSummary.acceleration().vector() -= R * activeMass.mass() * _g / pow<3>(r);
            if constexpr (PassiveOrder >= 2) {
                passiveSummary.acceleration().template tensor<2>() += D<2>(R, r) * activeMass.mass() * _g;
            }
            if constexpr (PassiveOrder >= 3) {
                passiveSummary.acceleration().template tensor<3>() += D<3>(R, r) * activeMass.mass() * _g / 2.0f;
                passiveSummary.acceleration().vector() +=
                        (D<3>(R, r) * activeSummary.moment().template tensor<2>()) / 2.0f;
            }
            if constexpr (PassiveOrder >= 4) {
                // todo: these coefficients were determined experimentally, and are probably not quite right!
                passiveSummary.acceleration().template tensor<4>() += D<4>(R, r) * activeMass.mass() * _g / 6.0f;
                passiveSummary.acceleration().template tensor<2>() -=
                        (D<4>(R, r) * activeSummary.moment().template tensor<2>()) / 2.0f;
                passiveSummary.acceleration().vector() +=
                        (D<4>(R, r) * activeSummary.moment().template tensor<3>()) / 6.0f;
            }
            if constexpr (PassiveOrder >= 5) {
                // todo
            }
            // todo: higher orders
        }


        template<std::size_t ActiveOrder>
        void nodeNode(
                const Position &activePosition,
                const MultipoleMassSummary<ActiveOrder> &activeSummary,
                const Position &passivePosition,
                AccelerationSummary &passiveSummary
        ) const {
            passiveSummary.acceleration() += nodeParticle(activePosition, activeSummary, passivePosition);
        }

    public:

        float &g() { return _g; }

        float &epsilon() { return _epsilon; }
    };

}

#endif //N_BODY_GRAVITY_H
