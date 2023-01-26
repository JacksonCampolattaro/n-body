//
// Created by Jackson Campolattaro on 1/26/23.
//

#ifndef N_BODY_DUALOCTREE_H
#define N_BODY_DUALOCTREE_H

#include "Octree.h"
#include "DualNode.h"

namespace NBody {

    class DualOctreeNode : public DualNode<OctreeNodeBase<DualOctreeNode>> {
    public:
        using DualNode::DualNode;
    };

    class DualOctree : public OctreeBase<DualOctreeNode> {
    public:
        using OctreeBase::OctreeBase;
    };
}


#endif //N_BODY_DUALOCTREE_H
