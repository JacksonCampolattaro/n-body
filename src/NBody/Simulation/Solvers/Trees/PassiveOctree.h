//
// Created by Jackson Campolattaro on 1/10/23.
//

#ifndef N_BODY_PASSIVEOCTREE_H
#define N_BODY_PASSIVEOCTREE_H

#include "Octree.h"
#include "PassiveNode.h"

namespace NBody {

    class PassiveOctreeNode : public PassiveNode<OctreeNodeBase<PassiveOctreeNode>> {
    public:

        using PassiveNode::PassiveNode;
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

        using OctreeBase::OctreeBase;
//        using OctreeBase::simulation;
//        using OctreeBase::root;
//        using OctreeBase::maxDepth;
//        using OctreeBase::maxLeafSize;
    };
}

#endif //N_BODY_PASSIVEOCTREE_H
