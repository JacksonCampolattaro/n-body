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
#include "Force.h"
#include "Mass.h"
#include "Quadrupole.h"
#include "SummaryType.h"

namespace NBody::Physics {

    class Rule {
    private:

        float _g;
        float _epsilon;

    public:

        explicit Rule(const float &G = 1.0, const float &epsilon = 0.0001) : _g(G), _epsilon(epsilon) {};

        Force operator()(const Position &activePosition, const Mass &activeMass,
                         const Position &passivePosition, const Mass &passiveMass) const {

            if (activePosition == passivePosition) return {};

            float force = _g * passiveMass.mass() * activeMass.mass() /
                          (glm::distance2((glm::vec3) activePosition, (glm::vec3) passivePosition) + _epsilon);

            return glm::normalize(activePosition - passivePosition) * force;
        }

        template<typename ActiveNode, typename PassiveNode>
        Acceleration operator()(const ActiveNode &activeNode,
                                const PassiveNode &passiveNode) const {

            // If the passive node has a center of mass, prefer that over the overall center
            Position passivePosition;
            if constexpr(requires(const PassiveNode &n) { n.summary().centerOfMass(); })
                passivePosition = passiveNode.summary().centerOfMass();
            else
                passivePosition = passiveNode.center();

            return operator()(activeNode, passivePosition);
        }

        template<typename ActiveNode>
        Acceleration operator()(const ActiveNode &activeNode,
                                const Position &passivePosition) const {

            // If the active node has a center of mass, prefer that over the overall center
            Position activePosition;
            if constexpr(requires(const ActiveNode &n) { n.summary().centerOfMass(); })
                activePosition = activeNode.summary().centerOfMass();
            else
                activePosition = activeNode.center();

            return operator()(activePosition, activeNode.summary(), passivePosition);
        }

        template<SummaryType ActiveSummary>
        Acceleration operator()(const Position &activePosition, const ActiveSummary &activeSummary,
                                const Position &passivePosition) const {

            // If the active summary provides a multipole moment, use that
            if constexpr(requires(const ActiveSummary &s){ s.moment(); })
                return operator()(activePosition, activeSummary.totalMass(), activeSummary.moment(), passivePosition);
            else
                return operator()(activePosition, activeSummary.totalMass(), passivePosition);
        }

        Acceleration operator()(const Position &activePosition, const Mass &activeMass,
                                const Position &passivePosition) const {

            if (activePosition == passivePosition) return {};

            // See: https://github.com/hannorein/rebound/blob/9fb9ee9aa20c547e1e6c67e7a58f07fd7176c181/src/gravity.c
            glm::vec3 differenceInPositions = passivePosition - activePosition;
            float r2 = glm::length2(differenceInPositions);
            float r = std::sqrt(r2 + _epsilon);
            float scaling = -_g * activeMass.mass() / (r * r * r);
            return scaling * differenceInPositions;
        }

        Acceleration operator()(const Position &activePosition, const Mass &activeMass, const Quadrupole &moment,
                                const Position &passivePosition) const {

            if (activePosition == passivePosition) return {};

            // See: https://github.com/hannorein/rebound/blob/9fb9ee9aa20c547e1e6c67e7a58f07fd7176c181/src/gravity.c
            glm::vec3 d = passivePosition - activePosition;
            float r2 = glm::length2(d);
            float r = std::sqrt(r2 + _epsilon);
            float scaling = -_g * activeMass.mass() / (r * r * r);

            float qScaling = _g / (r * r * r * r * r);

            float mrr = d.x * d.x * moment.xx() + d.y * d.y * moment.yy() + d.z * d.z * moment.zz() +
                        2.0f * (d.x * d.y * moment.xy() + d.x * d.z * moment.xz() + d.y * d.z * moment.yz());
            float combinedScaling = scaling + (qScaling * -5.0f / (2.0f * r * r) * mrr);

            return glm::vec3{
                    qScaling * (d.x * moment.xx() + d.y * moment.xy() + d.z * moment.xz()),
                    qScaling * (d.x * moment.xy() + d.y * moment.yy() + d.z * moment.yz()),
                    qScaling * (d.x * moment.xz() + d.y * moment.yz() + d.z * moment.zz())
            } + (d * combinedScaling);

        }

        Acceleration operator()(const Position &activePosition, const Mass &activeMass, const Quadrupole &activeMoment,
                                const Position &passivePosition, const Quadrupole &passiveMoment) const {

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
            glm::vec3 D1 = g1 * d;


            float g2 = fac2 * rinv2;
            glm::mat3 aux2 = glm::outerProduct(d, d);
            Quadrupole D2{aux2 * g2};
            D2.xx() += g1;
            D2.yy() += g1;
            D2.zz() += g1;

            // todo: Both acceleration vector & quadrupole acceleration must be saved to the node
            //TaylorCoeff[no_sink].coeff.dphi += mass_m * D1;
            //TaylorCoeff[no_sink].coeff.d2phi += (-mass_m) * D2;

            return D1;
        }

        float &g() { return _g; }

        float &epsilon() { return _epsilon; }
    };

}


//inline void
//fmm::fmm_node_node_interaction(int no_sink, int no_source, int type_sink, int type_source, unsigned char shmrank_sink,
//                               unsigned char shmrank_source, gravnode *noptr_sink, gravnode *noptr_source,
//                               vector <MyReal> &dxyz, MyReal &r2) {
//
//    /* 'sink' is a node with multipole moments
//     * 'source' node is a node with multipole moments
//     * 'dxyz' is the distance vector, pointing from sink to source, i.e. dxyz = pos(source) - pos(sink)
//     */
//
//    /* now do the node-node interaction */
//    MyReal r = sqrt(r2);
//
//    MyReal mass_m = noptr_source->mass;
//    MyReal mass_n = noptr_sink->mass;
//
//    MyReal rinv = (r > 0) ? 1 / r : 0;
//
//    MyReal h_n = All.ForceSoftening[noptr_sink->getSofteningClass()];
//    MyReal h_m = All.ForceSoftening[noptr_source->getSofteningClass()];
//    MyReal h_max = (h_m > h_n) ? h_m : h_n;
//
//    gfactors gfac;
//
//    get_gfactors_multipole(gfac, r, h_max, rinv);
//
//    MyReal g1 = gfac.fac1 * rinv;
//    vector <MyReal> D1 = g1 * dxyz;
//
//    MyReal g2 = gfac.fac2 * gfac.rinv2;
//    symtensor2 <MyReal> aux2 = dxyz % dxyz;  // construct outer product of the two vectors
//    symtensor2 <MyReal> D2 = g2 * aux2;
//    D2[qXX] += g1;
//    D2[qYY] += g1;
//    D2[qZZ] += g1;
//
//    TaylorCoeff[no_sink].coeff.dphi += mass_m * D1;
//    TaylorCoeff[no_sink].coeff.d2phi += (-mass_m) * D2;
//    TaylorCoeff[no_sink].coeff.interactions += 1;
//    interactioncountNN += 1;
//}


#endif //N_BODY_RULE_H
