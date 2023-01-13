//
// Created by Jackson Campolattaro on 1/10/23.
//

#ifndef N_BODY_FIELDOCTREE_H
#define N_BODY_FIELDOCTREE_H

#include "Octree.h"
#include "PassiveTreeNode.h"

namespace NBody {

    class FieldOctreeNode : public PassiveTreeNode<OctreeNodeBase<FieldOctreeNode>> {
    public:

        using PassiveTreeNode::PassiveTreeNode;
//        using PassiveTreeNode::children;
//        using PassiveTreeNode::boundingBox;
//        using PassiveTreeNode::center;
//        using PassiveTreeNode::sideLength;
//        using PassiveTreeNode::acceleration;
//        using PassiveTreeNode::summarize;
//        using PassiveTreeNode::collapseAccelerations;

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
