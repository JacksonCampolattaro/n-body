//
// Created by Jackson Campolattaro on 5/8/23.
//

#ifndef N_BODY_DUALTHETADIAGONALOVERDISTANCE_H
#define N_BODY_DUALTHETADIAGONALOVERDISTANCE_H

#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>

namespace NBody::Descent {

    class DualThetaDiagonalOverDistance : public DualThetaDescentCriterion {
    public:

        using DualThetaDescentCriterion::DualThetaDescentCriterion;

        template<typename TreeNode>
        inline bool operator()(const TreeNode &activeNode, const Position &point) const {
            return (activeNode.boundingBox().diagonalLength() /
                    glm::distance((glm::vec3) activeNode.center(), point))
                   < _momentTheta;
        }

        template<typename TreeNode>
        inline bool operator()(const Position &point, const TreeNode &passiveNode) const {
            return (passiveNode.boundingBox().diagonalLength() /
                    glm::distance((glm::vec3) passiveNode.center(), point))
                   < _momentTheta;
        }

        template<typename ActiveTreeNode, typename PassiveTreeNode>
        inline Recommendation operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {

            float activeDiagonal = activeNode.boundingBox().diagonalLength();
            float passiveDiagonal = passiveNode.boundingBox().diagonalLength();

            float distance = glm::distance((glm::vec3) activeNode.center(), passiveNode.center());

            // Descend the passive node, as long as it's more than half the size of the active node
            Recommendation shouldDescendPassive = (activeDiagonal / distance < _momentTheta) ?
                                                  Recommendation::DoNotDescend : Recommendation::DescendPassiveNode;

            // Descend the active node, as long as it's more than half the size of the passive node
            Recommendation shouldDescendActive = (passiveDiagonal / distance < _fieldTheta) ?
                                                 Recommendation::DoNotDescend : Recommendation::DescendActiveNode;

            return shouldDescendPassive | shouldDescendActive;

        }

    };

}
#endif //N_BODY_DUALTHETADIAGONALOVERDISTANCE_H
