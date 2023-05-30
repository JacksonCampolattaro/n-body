//
// Created by Jackson Campolattaro on 5/30/23.
//

#ifndef N_BODY_MAXSIDELENGTHOVERDISTANCE_H
#define N_BODY_MAXSIDELENGTHOVERDISTANCE_H

#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>

namespace NBody::Descent {

    class MaxSideLengthOverDistance : public ThetaDescentCriterion {
    public:

        using ThetaDescentCriterion::ThetaDescentCriterion;

        template<NodeType TreeNode>
        inline bool operator()(const TreeNode &activeNode, const Position &point) const {
            float distance = glm::distance((glm::vec3) activeNode.summary().centerOfMass(), point);
            return (activeNode.boundingBox().maxSideLength() / distance) < _theta
                   && !doIntersect(exclusionRegion(activeNode), point);
        }

        template<NodeType TreeNode>
        inline bool operator()(const Position &point, const TreeNode &passiveNode) const {
            float distance = glm::distance((glm::vec3) passiveNode.center(), point);
            return (3 * passiveNode.boundingBox().maxSideLength() / distance) < _theta
                   && !doIntersect(exclusionRegion(passiveNode), point);
        }

        template<NodeType ActiveTreeNode, NodeType PassiveTreeNode>
        inline Recommendation operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {

            float activeMaxSideLength = activeNode.boundingBox().maxSideLength();
            float passiveMaxSideLength = passiveNode.boundingBox().maxSideLength();

            float distance = glm::distance((glm::vec3) activeNode.summary().centerOfMass(), passiveNode.center());

            if (std::max(activeMaxSideLength, passiveMaxSideLength) / distance < _theta
                && !doIntersect(exclusionRegion(passiveNode), exclusionRegion(activeNode)))
                return Recommendation::Approximate;

            // Descend the passive node, as long as it's more than half the size of the active node
            Recommendation shouldDescendPassive = (activeMaxSideLength <= passiveMaxSideLength * 2.0f) ?
                                                  Recommendation::DescendPassiveNode : Recommendation::DoNotDescend;

            // Descend the active node, as long as it's more than half the size of the passive node
            Recommendation shouldDescendActive = (passiveMaxSideLength <= activeMaxSideLength * 2.0f) ?
                                                 Recommendation::DescendActiveNode : Recommendation::DoNotDescend;

            return shouldDescendPassive | shouldDescendActive;

        }

    };

}


#endif //N_BODY_MAXSIDELENGTHOVERDISTANCE_H
