//
// Created by jackcamp on 4/12/22.
//

#ifndef N_BODY_RULE_H
#define N_BODY_RULE_H

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>

#include <spdlog/spdlog.h>

#include "Position.h"
#include "Acceleration.h"
#include "Force.h"
#include "Mass.h"
#include "SummaryType.h"
#include "NBody/Physics/Summaries/MultipoleMassSummary.h"

#include <NBody/Physics/Summaries/CenterOfMassSummary.h>
#include <NBody/Physics/Summaries/AccelerationSummary.h>
#include <NBody/Physics/Summaries/QuadrupoleAccelerationSummary.h>

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
    inline SymmetricTensor3<Order> D(const glm::vec3 &R, float r) {

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
            C += 6.0f * SymmetricTensor3<4>::diagonal(R2.diagonal());
            C *= g3;

            return A + B + C;
        }
    }
}

namespace NBody::Physics {

    class Rule {
    private:

        float _g;
        float _epsilon;

    public:

        explicit Rule(const float &G = 1.0, const float &epsilon = 0.0001) : _g(G), _epsilon(epsilon) {};

    public: // Particle-particle interaction

        Acceleration operator()(const Position &activePosition,
                                const Mass &activeMass,
                                const Position &passivePosition) const {

            if (activePosition == passivePosition) return {};

            // See: https://github.com/hannorein/rebound/blob/9fb9ee9aa20c547e1e6c67e7a58f07fd7176c181/src/gravity.c
            glm::vec3 R = passivePosition - activePosition;
            float r = std::sqrt(glm::length2(R) + _epsilon);
            float scaling = -_g * activeMass.mass() / pow<3>(r);
            return scaling * R;
        }

    public: // Node-particle interaction

        template<typename ActiveNode>
        Acceleration operator()(const ActiveNode &activeNode,
                                const Position &passivePosition) const {

            // If the active node has a center of mass, prefer that over the overall center
            Position activePosition;
            if constexpr (requires(const ActiveNode &n) { n.summary().centerOfMass(); })
                activePosition = activeNode.summary().centerOfMass();
            else
                activePosition = activeNode.center();

            return operator()(activePosition, activeNode.summary(),
                              passivePosition);
        }

        template<ActiveSummaryType ActiveSummary>
        Acceleration operator()(const Position &activePosition,
                                const ActiveSummary &activeSummary,
                                const Position &passivePosition) const {

            // If the active node isn't multipolar, we can just treat it like a single particle
            return operator()(activePosition, activeSummary.totalMass(),
                              passivePosition);
        }

        template<std::size_t Order = 2>
        Acceleration operator()(const Position &activePosition,
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

            return _g * dPhi;
        }

    public: // Particle-node interaction

        template<typename PassiveNode>
        typename PassiveNode::Summary::Acceleration operator()(
                const Position &activePosition,
                const Mass &activeMass,
                const PassiveNode &passiveNode
        ) const {
            return operator()(activePosition, activeMass,
                              passiveNode.center(), passiveNode.summary());
        }

    public: // Node-node interaction

        template<typename ActiveNode, typename PassiveNode>
        typename PassiveNode::Summary::Acceleration operator()(
                const ActiveNode &activeNode,
                const PassiveNode &passiveNode
        ) const {

            // Self-interaction should never happen!
            assert((void *) &activeNode != (void *) &passiveNode);

            // If the active node has a center of mass, prefer that over the overall center
            Position activePosition;
            if constexpr (requires(const ActiveNode &n) { n.summary().centerOfMass(); })
                activePosition = activeNode.summary().centerOfMass();
            else
                activePosition = activeNode.center();

            return operator()(activePosition, activeNode.summary().totalMass(),
                              passiveNode.center(), passiveNode.summary());
        }

        Acceleration operator()(
                const Position &activePosition,
                const Mass &activeMass,
                const Position &passivePosition,
                const AccelerationSummary &passiveSummary
        ) const {
            return operator()(activePosition, activeMass, passivePosition);
        }

        MultipoleAcceleration<2> operator()(
                const Position &activePosition,
                const Mass &activeMass,
                const Position &passivePosition,
                const QuadrupoleAccelerationSummary &passiveSummary
        ) const {

            if (activePosition == passivePosition) return {};

            // See: https://github.com/weiguangcui/Gadget4/blob/7d3b425e3e0aa7b6b0e0dbefa1d4120c55980a8f/src/fmm/fmm.cc
            // "fmm_node_node_interaction"
            // todo: clean up and figure out what's going on with this math

            glm::vec3 R = passivePosition - activePosition;
            float r = std::sqrt(glm::length2(R) + _epsilon);

            auto D1_ = -R / pow<3>(r);

            // todo: is this the same as making it traceless?
            auto D2_ = SymmetricTensor3<2>::cartesianPower(R) * (-3.0 / pow<5>(r)) -
                       (SymmetricTensor3<2>::identity() / pow<3>(r));

            return MultipoleAcceleration<2>{
                    D1_ * activeMass.mass() * _g,
                    SymmetricTensor3<2>{D2_ * -activeMass.mass() * _g}
            };
        }

    public:

        float &g() { return _g; }

        float &epsilon() { return _epsilon; }
    };

}

#endif //N_BODY_RULE_H
