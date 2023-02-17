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

            // Self-interaction should never happen!
            assert((void *) &activeNode != (void *) &passiveNode);

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

            return (moment * d * qScaling) + (d * combinedScaling);

        }

        //        QuadrupoleAcceleration
        //        operator()(const Position &activePosition, const Mass &activeMass, const Quadrupole &activeMoment,
        //                   const Position &passivePosition) const {
        //
        //            if (activePosition == passivePosition) return {};
        //
        //            // See: https://github.com/weiguangcui/Gadget4/blob/7d3b425e3e0aa7b6b0e0dbefa1d4120c55980a8f/src/fmm/fmm.cc
        //            // "fmm_node_node_interaction"
        //
        //            glm::vec3 d = passivePosition - activePosition;
        //            float r2 = glm::length2(d);
        //            float r = std::sqrt(r2 + _epsilon);
        //
        //            float rinv = 1.0f / r;
        //            float rinv2 = rinv * rinv;
        //            float rinv3 = rinv2 * rinv;
        //
        //            float fac1 = 0.0f;
        //            float fac2 = 0.0f;
        //
        //            fac1 -= rinv2;
        //            fac2 += 3.0f * rinv3;
        //
        //            float g1 = fac1 * rinv;
        //            glm::vec3 D1 = g1 * d;
        //
        //
        //            float g2 = fac2 * rinv2;
        //            glm::mat3 aux2 = glm::outerProduct(d, d);
        //            Quadrupole D2{aux2 * g2};
        //            D2.xx() += g1;
        //            D2.yy() += g1;
        //            D2.zz() += g1;
        //
        //            // todo: Both acceleration vector & quadrupole acceleration must be saved to the node
        //            return {
        //                //activeMass * D1,
        //                //-activeMass * D2
        //            };
        //            //TaylorCoeff[no_sink].coeff.dphi += mass_m * D1;
        //            //TaylorCoeff[no_sink].coeff.d2phi += (-mass_m) * D2;
        //        }

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


///* function to account for interaction of two nodes in the tree */
//void fmm::fmm_force_interact(int no_sink, int no_source, char type_sink, char type_source, unsigned char shmrank_sink,
//                             unsigned char shmrank_source, int mintopleafnode, int committed) {
//
//    /* cell - cell interaction */
//    gravnode *noptr_sink = get_nodep(no_sink, shmrank_sink);
//    gravnode *noptr_source = get_nodep(no_source, shmrank_source);
//
//    /* both cells need to be non-empty */
//    if (noptr_sink->not_empty != 0 && noptr_source->not_empty != 0) {
//        {
//            MyReal r2;
//            vector <MyReal> dxyz;
//
//            int openflag = fmm_evaluate_node_node_opening_criterion(noptr_sink, noptr_source, dxyz, r2);
//
//            if (openflag == NODE_USE) {
//                /* evaluate the interaction */
//                fmm_node_node_interaction(no_sink, no_source, type_sink, type_source, shmrank_sink,
//                                          shmrank_source, noptr_sink,
//                                          noptr_source, dxyz, r2);
//            } else if (openflag == NODE_OPEN) {
//                /* open both */
//
//                if (no_source < MaxPart + MaxNodes)                  // we have a top-levelnode
//                    if (noptr_source->nextnode >=
//                        MaxPart + MaxNodes)  // if the next node is not a top-level, we have a leaf node
//                        mintopleafnode = std::min<int>(mintopleafnode, no_source);
//
//                if (no_sink < MaxPart + MaxNodes)                  // we have a top-levelnode
//                    if (noptr_sink->nextnode >=
//                        MaxPart + MaxNodes)  // if the next node is not a top-level, we have a leaf node
//                        mintopleafnode = std::min<int>(mintopleafnode, no_sink);
//
//                if (noptr_source->cannot_be_opened_locally || noptr_sink->cannot_be_opened_locally) {
//                    if (noptr_source->cannot_be_opened_locally && noptr_sink->cannot_be_opened_locally)
//                        Terminate("this should not happen, because then both nodes would be foreign");
//
//                    if (noptr_source->cannot_be_opened_locally)
//                        tree_add_to_fetch_stack(noptr_source, no_source, shmrank_source);
//
//                    if (noptr_sink->cannot_be_opened_locally)
//                        tree_add_to_fetch_stack(noptr_sink, no_sink, shmrank_sink);
//
//                    fmm_add_to_work_stack(no_source, no_sink, shmrank_source, shmrank_sink, mintopleafnode);
//                } else {
//                    int min_buffer_space =
//                            std::min<int>(MaxOnWorkStack - (NumOnWorkStack + NewOnWorkStack),
//                                          MaxOnFetchStack - NumOnFetchStack);
//
//                    if (min_buffer_space >=
//                        committed + 8 * 8 * TREE_NUM_BEFORE_NODESPLIT * TREE_NUM_BEFORE_NODESPLIT)
//                        fmm_open_both(noptr_sink, noptr_source, mintopleafnode,
//                                      committed +
//                                      8 * 8 * TREE_NUM_BEFORE_NODESPLIT * TREE_NUM_BEFORE_NODESPLIT);
//                    else
//                        fmm_add_to_work_stack(no_source, no_sink, shmrank_source, shmrank_sink, mintopleafnode);
//                }
//            }
//        }
//    }
//}

#endif //N_BODY_RULE_H
