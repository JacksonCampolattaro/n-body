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

        template<std::size_t Order = 2>
        Acceleration operator()(const Position &activePosition,
                                const MultipoleMassSummary<Order> &activeSummary,
                                const Position &passivePosition) const {

            using
            enum Dimension;

            if (activePosition == passivePosition) return {};

            // See: https://github.com/hannorein/rebound/blob/9fb9ee9aa20c547e1e6c67e7a58f07fd7176c181/src/gravity.c

            glm::vec3 R = passivePosition - activePosition;
            float r = std::sqrt(glm::length2(R) + _epsilon);

            float f1 = -1.0f / pow<2>(r);
            float f2 = 3.0f / pow<3>(r);
            float f3 = -15.0f / pow<4>(r);
            float f4 = 105.0f / pow<5>(r);
            float f5 = -945.0f / pow<6>(r);

            float g1 = f1 / r;
            glm::vec3 D1 = g1 * R;

            float g2 = f2 / pow<2>(r);
            SymmetricTensor3<2> D2 = g2 * SymmetricTensor3<2>::cartesianPower(R);
            D2.get<X, X>() += g1;
            D2.get<Y, Y>() += g1;
            D2.get<Z, Z>() += g1;

            float g3 = f3 / pow<3>(r);
            SymmetricTensor3<3> D3 = g3 * SymmetricTensor3<3>::cartesianPower(R);
            // todo: what is this operation?
            D3.get<X, X, X>() += 3 * g2 * R.x;
            D3.get<X, X, Y>() += g2 * R.y;
            D3.get<X, X, Z>() += g2 * R.z;
            D3.get<X, Y, Y>() += g2 * R.x;
            D3.get<X, Z, Z>() += g2 * R.x;
            D3.get<Y, Y, Y>() += 3 * g2 * R.y;
            D3.get<Y, Y, Z>() += g2 * R.z;
            D3.get<Y, Z, Z>() += g2 * R.y;
            D3.get<Z, Z, Z>() += 3 * g2 * R.z;

            glm::vec3 dPhi = activeSummary.totalMass().mass() * D1;
            //dPhi += 0.5 * (D3 * activeSummary.moment().tensor<2>());

            //return _g * dPhi;

            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

            float potential = -activeSummary.totalMass().mass() / pow<3>(r);
            potential += (activeSummary.moment().template tensor<2>() *
                          SymmetricTensor3<2>::cartesianPower(R)).sum() *
                         (-5.0f / (2.0f * pow<7>(r))); // todo: why is this -5 here?!

            auto forceVector = potential * R;
            forceVector += activeSummary.moment().template tensor<2>() * R / pow<5>(r);

            return _g * forceVector;
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

//#define MULTIPOLE_ORDER 3
//inline void fmm::fmm_particle_node_interaction(int no_sink, int no_source, int type_sink, int type_source,
//                                               unsigned char shmrank_sink,
//                                               unsigned char shmrank_source, gravnode *noptr_source,
//                                               vector <MyReal> &dxyz, MyReal &r2) {
//
//    /* 'sink' is a particle
//     * 'source' node is a node with multipole moments.
//     * 'dxyz' is the distance vector, pointing from sink to source, i.e. dxyz = pos(source) - pos(sink)
//     */
//
//    MyReal mass_i, h_i;
//
//    MyIntPosType *intpos_i;
//
//    if (type_sink == NODE_TYPE_LOCAL_PARTICLE) {
//        particle_data *P = get_Pp(no_sink, shmrank_sink);
//
//        intpos_i = P->IntPos;
//        mass_i = P->getMass();
//        h_i = All.ForceSoftening[P->getSofteningClass()];
//    } else if (type_sink == NODE_TYPE_TREEPOINT_PARTICLE) {
//        gravpoint_data *Pointp = get_pointsp(no_sink - ImportedNodeOffset, shmrank_sink);
//
//        intpos_i = Pointp->IntPos;
//        mass_i = Pointp->Mass;
//        h_i = All.ForceSoftening[Pointp->getSofteningClass()];
//    } else /* a point that was fetched */
//    {
//        foreign_gravpoint_data *foreignpoint = get_foreignpointsp(no_sink - EndOfForeignNodes, shmrank_sink);
//
//        intpos_i = foreignpoint->IntPos;
//        mass_i = foreignpoint->Mass;
//        h_i = All.ForceSoftening[foreignpoint->getSofteningClass()];
//    }
//
//    MyReal h_j = All.ForceSoftening[noptr_source->getSofteningClass()];
//    MyReal h_max = (h_j > h_i) ? h_j : h_i;
//
//    /* do cell-particle interaction, node can be used */
//    MyReal r = sqrt(r2);
//
//    MyReal rinv = (r > 0) ? 1 / r : 0;
//
//    gfactors gfac;
//
//
//    get_gfactors_multipole(gfac, r, h_max, rinv);
//
//    MyReal g1 = gfac.fac1 * rinv;
//    vector <MyReal> D1 = g1 * dxyz;
//
//#if(MULTIPOLE_ORDER >= 2)
//    MyReal g2 = gfac.fac2 * gfac.rinv2;
//    symtensor2 <MyReal> aux2 = dxyz % dxyz;  // construct outer product of the two vectors
//    symtensor2 <MyReal> D2 = g2 * aux2;
//    D2[qXX] += g1;
//    D2[qYY] += g1;
//    D2[qZZ] += g1;
//#endif
//#if(MULTIPOLE_ORDER >= 3)
//    MyReal g3 = gfac.fac3 * gfac.rinv3;
//    symtensor3 <MyReal> D3;
//    symtensor3 <MyReal> aux3;
//    setup_D3(INIT, D3, dxyz, aux2, aux3, g2, g3);
//#endif
//
//#if(MULTIPOLE_ORDER >= 4)
//    MyReal g4 = gfac.fac4 * gfac.rinv2 * gfac.rinv2;
//  symtensor4<MyReal> D4;
//  symtensor4<MyReal> aux4;
//  setup_D4(INIT, D4, dxyz, aux2, aux3, aux4, g2, g3, g4);
//#endif
//
//#if(MULTIPOLE_ORDER >= 5)
//    MyReal g5 = gfac.fac5 * gfac.rinv3 * gfac.rinv2;
//  symtensor5<MyReal> D5;
//  symtensor5<MyReal> aux5;
//  setup_D5(INIT, D5, dxyz, aux3, aux4, aux5, g3, g4, g5);
//#endif
//
//    /* finally store the force on the particle */
//    MyReal mass_j = noptr_source->mass;
//
//#if(MULTIPOLE_ORDER >= 3) || ((MULTIPOLE_ORDER >= 2) && defined(EXTRAPOTTERM))
//    symtensor2 <MyDouble> &Q2_j = noptr_source->Q2Tensor;
//#endif
//#if(MULTIPOLE_ORDER >= 4) || ((MULTIPOLE_ORDER >= 3) && defined(EXTRAPOTTERM))
//    symtensor3<MyDouble> &Q3_j = noptr_source->Q3Tensor;
//#endif
//#if(MULTIPOLE_ORDER >= 5) || ((MULTIPOLE_ORDER >= 4) && defined(EXTRAPOTTERM))
//    symtensor4<MyDouble> &Q4_j = noptr_source->Q4Tensor;
//#endif
//#if(MULTIPOLE_ORDER >= 5) && defined(EXTRAPOTTERM)
//    symtensor5<MyDouble> &Q5_j = noptr_source->Q5Tensor;
//#endif
//
//
//    vector <MyReal> dphi = mass_j * D1;
//
//#if(MULTIPOLE_ORDER >= 3)
//    dphi += static_cast<MyReal>(0.5) * (D3 * Q2_j);
//#endif
//#if(MULTIPOLE_ORDER >= 4)
//    dphi += static_cast<MyReal>(1.0 / 6) * (D4 * Q3_j);
//#endif
//#if(MULTIPOLE_ORDER >= 5)
//    dphi += static_cast<MyReal>(1.0 / 24) * (D5 * Q4_j);
//#endif
//
//    Tp->P[no_sink].GravAccel -= dphi;
//
//}

#endif //N_BODY_RULE_H
