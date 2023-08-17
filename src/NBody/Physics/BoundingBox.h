//
// Created by Jackson Campolattaro on 1/10/23.
//

#ifndef N_BODY_BOUNDINGBOX_H
#define N_BODY_BOUNDINGBOX_H

#include "Position.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace NBody {

    using Physics::Position;

    class BoundingBox {
    private:

        Position _min;
        Position _max;

    public:

        BoundingBox(const Position &min, const Position &max) : _min(min), _max(max) {}

        BoundingBox() :
                _min(
                        std::numeric_limits<float>::max(),
                        std::numeric_limits<float>::max(),
                        std::numeric_limits<float>::max()
                ),
                _max(
                        -std::numeric_limits<float>::max(),
                        -std::numeric_limits<float>::max(),
                        -std::numeric_limits<float>::max()
                ) {}

        [[nodiscard]] const Position &min() const { return _min; }

        Position &min() { return _min; }

        [[nodiscard]] const Position &max() const { return _max; }

        Position &max() { return _max; }

        [[nodiscard]] Position center() const { return (_max + _min) / 2.0f; }

        [[nodiscard]] glm::vec3 diagonal() const { return _max - _min; }

        [[nodiscard]] glm::vec3 dimensions() const { return diagonal(); }

        [[nodiscard]] float diagonalLength() const { return glm::distance((glm::vec3) _min, (glm::vec3) _max); }

        [[nodiscard]] float diagonalLength2() const { return glm::distance2((glm::vec3) _min, (glm::vec3) _max); }

        [[nodiscard]] float maxSideLength() const {
            return std::max({dimensions().x, dimensions().y, dimensions().z});
        }

        [[nodiscard]] bool contains(const Position &position) const {
            return glm::all(glm::lessThanEqual(min(), position)) &&
                   glm::all(glm::lessThan(position, max()));
        }

        friend std::ostream &operator<<(std::ostream &out, const BoundingBox &b) {
            return out << fmt::format("[({}, {}, {}), ({}, {}, {})]",
                                      b.min().x, b.min().y, b.min().z,
                                      b.max().x, b.max().y, b.max().z);
        }
    };

    [[nodiscard]] static bool doIntersect(const BoundingBox &b, const Position &p) {
        return b.contains(p);
    }

    [[nodiscard]] static bool doIntersect(const BoundingBox &b1, const BoundingBox &b2) {
        return glm::all(glm::greaterThanEqual(
                glm::min((glm::vec3) b1.max(), b2.max()),
                glm::max((glm::vec3) b1.min(), b2.min())
        ));
    }

    [[nodiscard]] static float separation(const BoundingBox &b1, const BoundingBox &b2) {
        auto u = glm::max((glm::vec3) b1.min() - b2.max(), 0.0f);
        auto v = glm::max((glm::vec3) b2.min() - b1.max(), 0.0f);
        return (float) std::sqrt(std::pow(glm::length(u), 2) + std::pow(glm::length(v), 2));
    }

    [[nodiscard]] static float separation(const BoundingBox &b, const Position &p) {
        auto u = glm::max((glm::vec3) b.min() - p, 0.0f);
        auto v = glm::max((glm::vec3) p - b.max(), 0.0f);
        return (float) std::sqrt(std::pow(glm::length(u), 2) + std::pow(glm::length(v), 2));
    }
}

#endif //N_BODY_BOUNDINGBOX_H
