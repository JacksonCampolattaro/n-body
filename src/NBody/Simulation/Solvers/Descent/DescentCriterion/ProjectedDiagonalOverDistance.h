//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_PROJECTEDDIAGONALOVERDISTANCE_H
#define N_BODY_PROJECTEDDIAGONALOVERDISTANCE_H

#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>

namespace NBody::Descent {

    class ProjectedDiagonalOverDistance : public ThetaDescentCriterion {
    public:

        using ThetaDescentCriterion::ThetaDescentCriterion;

        template<typename TreeNode>
        inline bool operator()(const TreeNode &node, const Position &point) const {

            glm::vec3 directionVector = point - node.summary().centerOfMass();

            // The diagonal of the bounding box always extends from min to max, bottom left to top right
            glm::vec3 diagonal = node.boundingBox().diagonal();

            // We want to make the direction vector roughly orthogonal, so it extends from top left to bottom right
            // This is done by explicitly setting its signs before normalizing
            auto reflectedUnitVector = glm::normalize(glm::abs(directionVector) * glm::vec3{-1.0, -1.0, 1.0});

            // Now we project the diagonal of the bounding box onto the unit vector
            glm::vec3 projectedDiagonal = diagonal - reflectedUnitVector * glm::dot(diagonal, reflectedUnitVector);

            // Use the length of this projected diagonal
            return (glm::length2(projectedDiagonal) / glm::length2(directionVector)) < (_theta * _theta);
        }

        template<typename ActiveTreeNode, typename PassiveTreeNode>
        inline bool operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {

            glm::vec3 directionVector = activeNode.summary().centerOfMass() - passiveNode.center();

            // The diagonal of the bounding box always extends from min to max, bottom left to top right
            glm::vec3 diagonalA = activeNode.boundingBox().diagonal();
            glm::vec3 diagonalP = passiveNode.boundingBox().diagonal();

            // We want to make the direction vector roughly orthogonal, so it extends from top left to bottom right
            // This is done by explicitly setting its signs before normalizing
            auto reflectedUnitVector = glm::normalize(glm::abs(directionVector) * glm::vec3{-1.0, -1.0, 1.0});

            // Now we project the diagonal of the bounding box onto the unit vector
            glm::vec3 projectedDiagonalA = diagonalA - reflectedUnitVector * glm::dot(diagonalA, reflectedUnitVector);
            glm::vec3 projectedDiagonalP = diagonalP - reflectedUnitVector * glm::dot(diagonalP, reflectedUnitVector);

            // Use the length of this projected diagonal
            return (std::max(glm::length2(projectedDiagonalA), glm::length2(projectedDiagonalP))
                    / glm::length2(directionVector)) < (_theta * _theta);
        }

    };

}

#endif //N_BODY_PROJECTEDDIAGONALOVERDISTANCE_H
