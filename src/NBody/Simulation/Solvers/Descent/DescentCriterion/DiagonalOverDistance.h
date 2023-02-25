//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_DIAGONALOVERDISTANCE_H
#define N_BODY_DIAGONALOVERDISTANCE_H

#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>

namespace NBody::Descent {

    class DiagonalOverDistance : public ThetaDescentCriterion {
    public:

        using ThetaDescentCriterion::ThetaDescentCriterion;

        template<typename TreeNode>
        inline bool operator()(const TreeNode &node, const Position &point) const {

            return (node.boundingBox().diagonalLength() /
                    glm::distance((glm::vec3) node.center(), point))
                   < _theta;
        }

        template<typename ActiveTreeNode, typename PassiveTreeNode>
        inline Recommendation operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {

            float activeDiagonal = activeNode.boundingBox().diagonalLength();
            float passiveDiagonal = passiveNode.boundingBox().diagonalLength();

            float distance = glm::distance((glm::vec3) activeNode.center(), passiveNode.center());

            if (std::max(activeDiagonal, passiveDiagonal) / distance < _theta)
                return Recommendation::Approximate;

            return activeDiagonal < passiveDiagonal ?
                   Recommendation::DescendPassiveNode : Recommendation::DescendActiveNode;

        }

    };

}

#endif //N_BODY_DIAGONALOVERDISTANCE_H
