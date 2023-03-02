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

#include <NBody/Physics/Summaries/CenterOfMassSummary.h>
#include <NBody/Physics/Summaries/QuadrupoleMassSummary.h>
#include <NBody/Physics/Summaries/AccelerationSummary.h>
#include <NBody/Physics/Summaries/QuadrupoleAccelerationSummary.h>

namespace {

    template<std::size_t P, typename T>
    inline T pow(T value) {
        if constexpr (P == 0)
            return 1;
        else
            return value * pow<P - 1>(value);
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

        template<QuadrupoleActiveSummaryType ActiveSummary>
        Acceleration operator()(const Position &activePosition,
                                const ActiveSummary &activeSummary,
                                const Position &passivePosition) const {

            if (activePosition == passivePosition) return {};

            // See: https://github.com/hannorein/rebound/blob/9fb9ee9aa20c547e1e6c67e7a58f07fd7176c181/src/gravity.c
            // todo: more descriptive variable names

            glm::vec3 R = passivePosition - activePosition;
            float r = std::sqrt(glm::length2(R) + _epsilon);

            float monopolePotential = -activeSummary.totalMass().mass() / pow<3>(r);
            float quadrupolePotential = (activeSummary.moment() * SymmetricTensor3<2>::cartesianPower(R)).sum()
                                        * (-5.0f / (2.0f * pow<7>(r)));
            float combinedPotential = monopolePotential + quadrupolePotential;

            return _g * ((activeSummary.moment() * R / pow<5>(r)) + (R * combinedPotential));
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
            auto D2_ = SymmetricTensor3<2>::cartesianPower(R) * (3.0 / pow<5>(r)) -
                       (SymmetricTensor3<2>::identity() / pow<3>(r));

            return {
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
