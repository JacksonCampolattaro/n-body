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

        template<typename TreeNode>
        inline bool operator()(const TreeNode &node, const Position &point) const {
            float distance = glm::distance((glm::vec3) node.center(), point);
            return (2.0f * node.sideLength() / distance) < _theta;
        }

        template<typename ActiveTreeNode, typename PassiveTreeNode>
        inline Recommendation operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {

            float activeSideLength = activeNode.sideLength();
            float passiveSideLength = passiveNode.sideLength();

            // todo: maybe I should always use center? centerOfMass might be offset
            float distance = glm::distance((glm::vec3) activeNode.center(), passiveNode.center());

            if ((2.0f * std::max(activeSideLength, passiveSideLength) / distance) < _theta)
                return Recommendation::Approximate;

            return activeSideLength < passiveSideLength ?
                   Recommendation::DescendPassiveNode : Recommendation::DescendActiveNode;

        }

    };
}

#endif //N_BODY_SIDELENGTHOVERDISTANCE_H
