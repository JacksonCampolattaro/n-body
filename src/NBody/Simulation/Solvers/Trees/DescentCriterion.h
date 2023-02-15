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
            return (2.0f * node.sideLength() / glm::distance((glm::vec3) node.summary().centerOfMass(), point))
                   < _theta;
        }

        template<typename ActiveTreeNode, typename PassiveTreeNode>
        inline bool operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {
            if constexpr(requires(const ActiveTreeNode &n) { n.sideLength(); })
                return (std::max(activeNode.sideLength(), passiveNode.sideLength()) /
                        glm::distance((glm::vec3) activeNode.summary().centerOfMass(), passiveNode.center())) < _theta;
            else
                return (std::max(activeNode.boundingBox().maxSideLength(), passiveNode.sideLength()) /
                        glm::distance((glm::vec3) activeNode.summary().centerOfMass(), passiveNode.center())) < _theta;
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
            return (node.boundingBox().diagonalLength() /
                    glm::distance((glm::vec3) node.summary().centerOfMass(), point))
                   < _theta;
        }

        template<typename ActiveTreeNode, typename PassiveTreeNode>
        inline bool operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {
            return (std::max(activeNode.boundingBox().diagonalLength(), passiveNode.boundingBox().diagonalLength()) /
                    glm::distance((glm::vec3) activeNode.summary().centerOfMass(), passiveNode.center())) < _theta;
            //            if constexpr(requires(const PassiveTreeNode &n) { n.sideLength(); })
            //                return (std::max(activeNode.boundingBox().diagonalLength(), passiveNode.sideLength()) /
            //                        glm::distance((glm::vec3) activeNode.summary().centerOfMass(), passiveNode.center())) < _theta;
            //            else
            //                return (std::max(activeNode.boundingBox().diagonalLength(), passiveNode.boundingBox().diagonalLength()) /
            //                        glm::distance((glm::vec3) activeNode.summary().centerOfMass(), passiveNode.center())) < _theta;
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

        [[nodiscard]] const float &theta() const { return _theta; }

        float &theta() { return _theta; }
    };

}

#endif //N_BODY_DESCENTCRITERION_H
