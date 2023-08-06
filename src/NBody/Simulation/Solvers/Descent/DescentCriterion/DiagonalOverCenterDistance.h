//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_DIAGONALOVERCENTERDISTANCE_H
#define N_BODY_DIAGONALOVERCENTERDISTANCE_H

#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <glm/gtx/norm.hpp>

namespace NBody::Descent {

    class DiagonalOverCenterDistance : public ThetaDescentCriterion {
    public:

        using ThetaDescentCriterion::ThetaDescentCriterion;

        template<NodeType TreeNode>
        inline bool operator()(const TreeNode &activeNode, const Position &point) const {
            //float distance2 = glm::distance2((glm::vec3) activeNode.center(), point);
            float distance2 = std::min(
                    glm::distance2((glm::vec3) activeNode.center(), point),
                    glm::distance2((glm::vec3) activeNode.summary().centerOfMass(), point)
            );
            return (activeNode.boundingBox().diagonalLength2() / distance2) < (_theta * _theta)
                   && activeNode.contents().size() >= 32;
        }

        template<NodeType TreeNode>
        inline bool operator()(const Position &point, const TreeNode &passiveNode) const {
            float distance = glm::distance((glm::vec3) passiveNode.center(), point);
            return (passiveNode.boundingBox().diagonalLength() / distance) < _theta
                   //&& !doIntersect(exclusionRegion(passiveNode), point)
                   && passiveNode.contents().size() >= 32;
        }

        template<NodeType ActiveTreeNode, NodeType PassiveTreeNode>
        inline Recommendation operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {

            float activeDiagonal2 = activeNode.boundingBox().diagonalLength2();
            float passiveDiagonal2 = passiveNode.boundingBox().diagonalLength2();

            float distance2 = std::min(
                    glm::distance2((glm::vec3) activeNode.center(), passiveNode.center()),
                    glm::distance2((glm::vec3) activeNode.summary().centerOfMass(), passiveNode.center())
            );

            if (
                    2 * std::max(activeDiagonal2, passiveDiagonal2) / distance2 < (_theta * _theta)
                    && !doIntersect(exclusionRegion(passiveNode), exclusionRegion(activeNode))
                    && activeNode.contents().size() >= 32
                    //&& passiveNode.contents().size() >= 64
                    )
                return Recommendation::Approximate;

            // Descend the passive node, as long as it's more than half the size of the active node
            Recommendation shouldDescendPassive = (activeDiagonal2 <= passiveDiagonal2 * 4.0f) ?
                                                  Recommendation::DescendPassiveNode : Recommendation::DoNotDescend;

            // Descend the active node, as long as it's more than half the size of the passive node
            Recommendation shouldDescendActive = (passiveDiagonal2 <= activeDiagonal2 * 4.0f) ?
                                                 Recommendation::DescendActiveNode : Recommendation::DoNotDescend;

            return shouldDescendPassive | shouldDescendActive;

        }

    };

}

#endif //N_BODY_DIAGONALOVERCENTERDISTANCE_H
