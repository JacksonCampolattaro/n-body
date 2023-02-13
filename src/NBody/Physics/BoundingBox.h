//
// Created by Jackson Campolattaro on 1/10/23.
//

#ifndef N_BODY_BOUNDINGBOX_H
#define N_BODY_BOUNDINGBOX_H

#include "Position.h"

#include <glm/glm.hpp>

namespace NBody {

    using Physics::Position;

    class BoundingBox {
    private:

        Position _min;
        Position _max;

    public:

        BoundingBox(
                Position min = {
                        std::numeric_limits<float>::max(),
                        std::numeric_limits<float>::max(),
                        std::numeric_limits<float>::max()
                },
                Position max = {
                        -std::numeric_limits<float>::max(),
                        -std::numeric_limits<float>::max(),
                        -std::numeric_limits<float>::max()
                }
        ) : _min(min), _max(max) {}

        [[nodiscard]] const Position &min() const { return _min; }

        Position &min() { return _min; }

        [[nodiscard]] const Position &max() const { return _max; }

        Position &max() { return _max; }

        Position center() const { return (_max + _min) / 2.0f; }

        [[nodiscard]] glm::vec3 diagonal() const { return _max - _min; }

        [[nodiscard]] glm::vec3 dimensions() const { return diagonal(); }

        [[nodiscard]] float diagonalLength() const { return glm::distance((glm::vec3) _min, (glm::vec3) _max); }

        [[nodiscard]] float maxSideLength() const {
            return std::max(std::max(dimensions().x, dimensions().y), dimensions().z);
        }

        [[nodiscard]] bool contains(const Position &position) const {
            return glm::all(glm::greaterThanEqual(min(), position)) &&
                   glm::all(glm::greaterThanEqual(position, max()));
        }
    };
}

#endif //N_BODY_BOUNDINGBOX_H
