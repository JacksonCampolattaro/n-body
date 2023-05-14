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
            return (activeNode.sideLength() / distance) < _theta && distance > activeNode.sideLength();
        }

        template<NodeType TreeNode>
        inline bool operator()(const Position &point, const TreeNode &passiveNode) const {
            float distance = glm::distance((glm::vec3) passiveNode.center(), point);
            return (passiveNode.sideLength() / distance) < _theta;
        }

        template<NodeType ActiveTreeNode, NodeType PassiveTreeNode>
        inline Recommendation operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {

            float activeSideLength = activeNode.sideLength();
            float passiveSideLength = passiveNode.sideLength();

            // todo: maybe I should always use center? centerOfMass might be offset
            float distance = glm::distance((glm::vec3) activeNode.summary().centerOfMass(), passiveNode.center());

            // "Exclusion region" from GADGET-4
            // todo: this math is kinda wrong
            auto centerToCenter = activeNode.center() - passiveNode.center();
            if (std::max({centerToCenter.x, centerToCenter.y, centerToCenter.z}) <
                std::max(activeSideLength, passiveSideLength))
                return Recommendation::DescendBothNodes;

            if ((std::max(activeSideLength, passiveSideLength) / distance) < _theta)
                return Recommendation::Approximate;

            // Descend the passive node, as long as it's more than half the size of the active node
            Recommendation shouldDescendPassive = (activeNode.sideLength() < passiveNode.sideLength() * 2.0f) ?
                                                  Recommendation::DescendPassiveNode : Recommendation::DoNotDescend;

            // Descend the active node, as long as it's more than half the size of the passive node
            Recommendation shouldDescendActive = (passiveNode.sideLength() < activeNode.sideLength() * 2.0f) ?
                                                 Recommendation::DescendActiveNode : Recommendation::DoNotDescend;

            return shouldDescendPassive | shouldDescendActive;

        }

    };
}

#endif //N_BODY_SIDELENGTHOVERDISTANCE_H
