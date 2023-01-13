//
// Created by Jackson Campolattaro on 1/10/23.
//

#ifndef N_BODY_FIELDOCTREE_H
#define N_BODY_FIELDOCTREE_H

#include "Octree.h"
#include "PassiveNode.h"

namespace NBody {

    class FieldOctreeNode : public PassiveNode<OctreeNodeBase<FieldOctreeNode>> {
    public:

        using PassiveNode::PassiveNode;
//        using PassiveNode::children;
//        using PassiveNode::boundingBox;
//        using PassiveNode::center;
//        using PassiveNode::sideLength;
//        using PassiveNode::acceleration;
//        using PassiveNode::summarize;
//        using PassiveNode::collapseAccelerations;

    };

    class FieldOctree : public OctreeBase<FieldOctreeNode> {
    public:

        using OctreeBase::OctreeBase;
//        using OctreeBase::simulation;
//        using OctreeBase::root;
//        using OctreeBase::maxDepth;
//        using OctreeBase::maxLeafSize;
    };
}

#endif //N_BODY_FIELDOCTREE_H
