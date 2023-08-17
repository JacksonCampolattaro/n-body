//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_SIDELENGTHOVERDISTANCE_H
#define N_BODY_SIDELENGTHOVERDISTANCE_H

#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>

namespace NBody::Descent {

    class SideLengthOverDistance : public ThetaDescentCriterion {
    public:

        using ThetaDescentCriterion::ThetaDescentCriterion;

        template<NodeType TreeNode>
        inline bool operator()(const TreeNode &activeNode, const Position &point) const {
            float distance = glm::distance((glm::vec3) activeNode.summary().centerOfMass(), point);
            return (activeNode.sideLength() / distance) < _theta
                   && !doIntersect(exclusionRegion(activeNode), point);
        }

        template<NodeType TreeNode>
        inline bool operator()(const Position &point, const TreeNode &passiveNode) const {
            float distance = glm::distance((glm::vec3) passiveNode.center(), point);
            return (2 * passiveNode.sideLength() / distance) < _theta
                   && !doIntersect(passiveNode.boundingBox(), point);
        }

        template<NodeType ActiveTreeNode, NodeType PassiveTreeNode>
        inline Recommendation operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {

            float activeSideLength2 = activeNode.sideLength() * activeNode.sideLength();
            float passiveSideLength2 = passiveNode.sideLength() * passiveNode.sideLength();

            float distance2 = glm::distance2((glm::vec3) activeNode.summary().centerOfMass(), passiveNode.center());

            if ((4 * std::max(activeSideLength2, passiveSideLength2) / distance2) < (_theta * _theta)
                && !doIntersect(exclusionRegion(passiveNode), exclusionRegion(activeNode)))
                return Recommendation::Approximate;

            // Descend the passive node, as long as it's more than half the size of the active node
            Recommendation shouldDescendPassive = (activeSideLength2 <= passiveSideLength2 * 4.0f) ?
                                                  Recommendation::DescendPassiveNode : Recommendation::DoNotDescend;

            // Descend the active node, as long as it's more than half the size of the passive node
            Recommendation shouldDescendActive = (passiveSideLength2 <= activeSideLength2 * 4.0f) ?
                                                 Recommendation::DescendActiveNode : Recommendation::DoNotDescend;

            return shouldDescendPassive | shouldDescendActive;

        }

    };
}

#endif //N_BODY_SIDELENGTHOVERDISTANCE_H
