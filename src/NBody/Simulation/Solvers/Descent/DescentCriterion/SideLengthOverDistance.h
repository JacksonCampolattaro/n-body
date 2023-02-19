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
            return (2.0f * node.sideLength() / glm::distance((glm::vec3) node.summary().centerOfMass(), point))
                   < _theta;
        }

        template<typename ActiveTreeNode, typename PassiveTreeNode>
        inline bool operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {
            return (2.0f * std::max(activeNode.sideLength(), passiveNode.sideLength()) /
                    glm::distance((glm::vec3) activeNode.summary().centerOfMass(), passiveNode.center())) < _theta;
        }

    };
}

#endif //N_BODY_SIDELENGTHOVERDISTANCE_H
