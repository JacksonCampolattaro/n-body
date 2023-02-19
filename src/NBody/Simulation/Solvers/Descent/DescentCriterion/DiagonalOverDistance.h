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
                    glm::distance((glm::vec3) node.summary().centerOfMass(), point))
                   < _theta;
        }

        template<typename ActiveTreeNode, typename PassiveTreeNode>
        inline bool operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {
            return (std::max(activeNode.boundingBox().diagonalLength(), passiveNode.boundingBox().diagonalLength()) /
                    glm::distance((glm::vec3) activeNode.summary().centerOfMass(), passiveNode.center())) < _theta;
        }

    };

}

#endif //N_BODY_DIAGONALOVERDISTANCE_H
