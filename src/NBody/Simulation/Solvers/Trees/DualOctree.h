//
// Created by Jackson Campolattaro on 1/26/23.
//

#ifndef N_BODY_DUALOCTREE_H
#define N_BODY_DUALOCTREE_H

#include "Octree.h"
#include "Summaries/DualSummary.h"

namespace NBody {

    class DualOctreeNode : public OctreeNodeBase<DualOctreeNode, DualSummary> {
    public:
        using OctreeNodeBase::OctreeNodeBase;
    };

    class DualOctree : public OctreeBase<DualOctreeNode> {
    public:
        using OctreeBase::OctreeBase;
    };
}


#endif //N_BODY_DUALOCTREE_H
