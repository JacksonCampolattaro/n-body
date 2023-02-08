//
// Created by Jackson Campolattaro on 1/10/23.
//

#ifndef N_BODY_PASSIVEOCTREE_H
#define N_BODY_PASSIVEOCTREE_H

#include "Octree.h"

#include <NBody/Simulation/Solvers/Trees/Summaries/AccelerationSummary.h>

namespace NBody {

    class PassiveOctreeNode : public OctreeNodeBase<PassiveOctreeNode, AccelerationSummary> {
    public:

        using OctreeNodeBase::OctreeNodeBase;
//        using PassiveNode::children;
//        using PassiveNode::activeBoundingBox;
//        using PassiveNode::center;
//        using PassiveNode::sideLength;
//        using PassiveNode::force;
//        using PassiveNode::summarize;
//        using PassiveNode::collapseAccelerations;

    };

    class PassiveOctree : public OctreeBase<PassiveOctreeNode> {
    public:

        using OctreeBase::Node;

        using OctreeBase::OctreeBase;
    };
}

#endif //N_BODY_PASSIVEOCTREE_H
