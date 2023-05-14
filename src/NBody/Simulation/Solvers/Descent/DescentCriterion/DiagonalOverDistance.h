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

        template<NodeType TreeNode>
        inline bool operator()(const TreeNode &activeNode, const Position &point) const {

            return (activeNode.boundingBox().diagonalLength() /
                    glm::distance((glm::vec3) activeNode.center(), point))
                   < _theta;
        }

        template<NodeType TreeNode>
        inline bool operator()(const Position &point, const TreeNode &passiveNode) const {

            return (passiveNode.boundingBox().diagonalLength() /
                    glm::distance((glm::vec3) passiveNode.center(), point))
                   < _theta;
        }

        template<NodeType ActiveTreeNode, NodeType PassiveTreeNode>
        inline Recommendation operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {

            float activeDiagonal = activeNode.boundingBox().diagonalLength();
            float passiveDiagonal = passiveNode.boundingBox().diagonalLength();

            float distance = glm::distance((glm::vec3) activeNode.summary().centerOfMass(), passiveNode.center());

            // "Exclusion region" from GADGET-4
            // todo: might be able to use something different for non-cubic nodes!
            auto centerToCenter = activeNode.center() - passiveNode.center();
            if (std::max({centerToCenter.x, centerToCenter.y, centerToCenter.z}) <
                activeNode.boundingBox().maxSideLength() + passiveNode.sideLength())
                return Recommendation::DescendBothNodes;

            if (std::max(activeDiagonal, passiveDiagonal) / distance < _theta)
                return Recommendation::Approximate;

            // Descend the passive node, as long as it's more than half the size of the active node
            Recommendation shouldDescendPassive = (activeDiagonal < passiveDiagonal * 2.0f) ?
                                                  Recommendation::DescendPassiveNode : Recommendation::DoNotDescend;

            // Descend the active node, as long as it's more than half the size of the passive node
            Recommendation shouldDescendActive = (passiveDiagonal < activeDiagonal * 2.0f) ?
                                                 Recommendation::DescendActiveNode : Recommendation::DoNotDescend;

            return shouldDescendPassive | shouldDescendActive;

        }

    };

}

#endif //N_BODY_DIAGONALOVERDISTANCE_H
