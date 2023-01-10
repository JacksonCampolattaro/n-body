//
// Created by Jackson Campolattaro on 1/10/23.
//

#ifndef N_BODY_DESCENTCRITERION_H
#define N_BODY_DESCENTCRITERION_H

#include <NBody/Physics/Position.h>

#include <glm/glm.hpp>

namespace NBody::DescentCriterion {

    // todo: a c++20 "DescentCriterion" concept would be appropriate here

    using Physics::Position;

    class SideLengthOverDistance {
    private:

        float _theta;

    public:

        explicit SideLengthOverDistance(float theta = 0.5f) : _theta(theta) {}

        template<typename TreeNode>
        inline bool operator()(const TreeNode &node, const Position &point) const {
            return (node.sideLength() / glm::distance((glm::vec3) node.centerOfMass(), point))
                   < _theta;
        }

        [[nodiscard]] const float &theta() const { return _theta; }

        float &theta() { return _theta; }
    };

    class DiagonalOverDistance {

    private:

        float _theta;

    public:

        explicit DiagonalOverDistance(float theta = 0.5f) : _theta(theta) {}

        template<typename TreeNode>
        inline bool operator()(const TreeNode &node, const Position &point) const {
            return (node.boundingBox().diagonalLength() / glm::distance((glm::vec3) node.centerOfMass(), point))
                   < _theta;
        }

        [[nodiscard]] const float &theta() const { return _theta; }

        float &theta() { return _theta; }
    };

    class ProjectedDiagonalOverDistance {

    private:

        float _theta;

    public:

        explicit ProjectedDiagonalOverDistance(float theta = 0.5f) : _theta(theta) {}

        template<typename TreeNode>
        inline bool operator()(const TreeNode &node, const Position &point) const {

            glm::vec3 directionVector = point - node.centerOfMass();

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

        [[nodiscard]] const float &theta() const { return _theta; }

        float &theta() { return _theta; }
    };

}

#endif //N_BODY_DESCENTCRITERION_H
