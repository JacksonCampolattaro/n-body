//
// Created by Jackson Campolattaro on 1/13/23.
//

#ifndef N_BODY_COMPREHENSIVETREENODE_H
#define N_BODY_COMPREHENSIVETREENODE_H

#include "ActiveNode.h"
#include "PassiveNode.h"

namespace NBody {

    template<typename TreeNode>
    class ComprehensiveTreeNode : public ActiveNode<PassiveNode<TreeNode>> {
    public:

        using TreeNode::TreeNode;
        using TreeNode::isLeaf;
        using TreeNode::contents;
        using TreeNode::children;
        using TreeNode::boundingBox;

        static std::vector<Entity> relevantEntities(Simulation &simulation) {
            return {simulation.group<const Position>().begin(),
                    simulation.group<const Position>().end()};
        }

        // todo: summarization won't work properly without some changes
        // (Active & Passive nodes need different arguments when splitting)
    };

}

#endif //N_BODY_COMPREHENSIVETREENODE_H
