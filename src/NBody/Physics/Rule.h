//
// Created by jackcamp on 4/12/22.
//

#ifndef N_BODY_RULE_H
#define N_BODY_RULE_H

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>

#include "Position.h"
#include "Acceleration.h"
#include "QuadrupoleAcceleration.h"
#include "Force.h"
#include "Mass.h"
#include "Quadrupole.h"
#include "SummaryType.h"

#include <NBody/Physics/Summaries/CenterOfMassSummary.h>
#include <NBody/Physics/Summaries/QuadrupoleMassSummary.h>
#include <NBody/Physics/Summaries/AccelerationSummary.h>
#include <NBody/Physics/Summaries/QuadrupoleAccelerationSummary.h>

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
            glm::vec3 differenceInPositions = passivePosition - activePosition;
            float r2 = glm::length2(differenceInPositions);
            float r = std::sqrt(r2 + _epsilon);
            float scaling = -_g * activeMass.mass() / (r * r * r);
            return scaling * differenceInPositions;
        }

    public: // Node-particle interaction

        template<typename ActiveNode>
        Acceleration operator()(const ActiveNode &activeNode,
                                const Position &passivePosition) const {

            // If the active node has a center of mass, prefer that over the overall center
            Position activePosition;
            if constexpr(requires(const ActiveNode &n) { n.summary().centerOfMass(); })
                activePosition = activeNode.summary().centerOfMass();
            else
                activePosition = activeNode.center();

            return operator()(activePosition, activeNode.summary(),
                              passivePosition);
        }

        Acceleration operator()(const Position &activePosition,
                                const CenterOfMassSummary &activeSummary,
                                const Position &passivePosition) const {

            // If the active node isn't multipolar, we can just treat it like a single particle
            return operator()(activePosition, activeSummary.totalMass(),
                              passivePosition);
        }

        Acceleration operator()(const Position &activePosition,
                                const QuadrupoleMassSummary &activeSummary,
                                const Position &passivePosition) const {

            if (activePosition == passivePosition) return {};

            // See: https://github.com/hannorein/rebound/blob/9fb9ee9aa20c547e1e6c67e7a58f07fd7176c181/src/gravity.c
            glm::vec3 d = passivePosition - activePosition;
            float r2 = glm::length2(d);
            float r = std::sqrt(r2 + _epsilon);
            float scaling = -_g * activeSummary.totalMass().mass() / (r * r * r);

            float qScaling = _g / (r * r * r * r * r);

            // todo: maybe this could be better expressed as a matrix-vector multiplication?
            float mrr = d.x * d.x * activeSummary.moment().xx() +
                        d.y * d.y * activeSummary.moment().yy() +
                        d.z * d.z * activeSummary.moment().zz() +
                        2.0f * (
                                d.x * d.y * activeSummary.moment().xy() +
                                d.x * d.z * activeSummary.moment().xz() +
                                d.y * d.z * activeSummary.moment().yz()
                        );
            float combinedScaling = scaling + (qScaling * -5.0f / (2.0f * r * r) * mrr);

            return (activeSummary.moment() * d * qScaling) + (d * combinedScaling);

            //            auto a = operator()(
            //                    activePosition, activeSummary,
            //                    passivePosition, QuadrupoleAccelerationSummary{}
            //            ).acceleration();
            //            spdlog::error("{}, {}, {}  vs  {}, {}, {}",
            //                          a.x, a.y, a.z,
            //                          A.x, A.y, A.z);
        }

        template<SummaryType ActiveSummary>
        Acceleration operator()(const Position &activePosition,
                                const ActiveSummary &activeSummary,
                                const Position &passivePosition) const {
            spdlog::error("Interaction is not implemented for this active node type!");
            return {};
        }

    public: // Node-node interaction

        template<typename ActiveNode, typename PassiveNode>
        typename PassiveNode::Summary::Acceleration operator()(
                const ActiveNode &activeNode,
                const PassiveNode &passiveNode
        ) const {

            // Self-interaction should never happen!
            assert((void *) &activeNode != (void *) &passiveNode);

            // If the passive node has a center of mass, prefer that over the overall center
            Position passivePosition;
            if constexpr(requires(const PassiveNode &n) { n.summary().centerOfMass(); })
                passivePosition = passiveNode.summary().centerOfMass();
            else
                passivePosition = passiveNode.center();

            // The same applies to the active node
            Position activePosition;
            if constexpr(requires(const ActiveNode &n) { n.summary().centerOfMass(); })
                activePosition = activeNode.summary().centerOfMass();
            else
                activePosition = activeNode.center();

            return operator()(activePosition, activeNode.summary(),
                              passivePosition, passiveNode.summary());
        }

        template<SummaryType ActiveSummary>
        Acceleration operator()(
                const Position &activePosition,
                const ActiveSummary &activeSummary,
                const Position &passivePosition,
                const AccelerationSummary &passiveSummary
        ) const {
            return operator()(activePosition, activeSummary.totalMass(),
                              passivePosition);
        }

        template<SummaryType ActiveSummary>
        QuadrupoleAcceleration operator()(
                const Position &activePosition,
                const ActiveSummary &activeSummary,
                const Position &passivePosition,
                const QuadrupoleAccelerationSummary &passiveSummary
        ) const {

            if (activePosition == passivePosition) return {};

            // See: https://github.com/weiguangcui/Gadget4/blob/7d3b425e3e0aa7b6b0e0dbefa1d4120c55980a8f/src/fmm/fmm.cc
            // "fmm_node_node_interaction"

            glm::vec3 d = passivePosition - activePosition;
            float r2 = glm::length2(d);
            float r = std::sqrt(r2 + _epsilon);

            float rinv = 1.0f / r;
            float rinv2 = rinv * rinv;
            float rinv3 = rinv2 * rinv;

            float fac1 = 0.0f;
            float fac2 = 0.0f;

            fac1 -= rinv2;
            fac2 += 3.0f * rinv3;

            float g1 = fac1 * rinv;
            glm::vec3 D1 = d * g1 * _g;


            float g2 = fac2 * rinv2;
            glm::mat3 d2 = glm::outerProduct(d, d);
            Quadrupole D2{d2 * g2};
            D2.xx() += g1;
            D2.yy() += g1;
            D2.zz() += g1;

            return {
                    D1 * activeSummary.totalMass().mass(),
                    D2 * -activeSummary.totalMass().mass()
            };
        }

    public:

        float &g() { return _g; }

        float &epsilon() { return _epsilon; }
    };

}

#endif //N_BODY_RULE_H
