//
// Created by Jackson Campolattaro on 2/15/23.
//

#ifndef N_BODY_QUADRUPOLEACCELERATION_H
#define N_BODY_QUADRUPOLEACCELERATION_H

#include <NBody/Physics/Acceleration.h>
#include <NBody/Physics/Quadrupole.h>

namespace NBody::Physics {

    class QuadrupoleAcceleration {
    private:

        Acceleration _acceleration{};
        Quadrupole _quadrupole{};

    public:

        QuadrupoleAcceleration() = default;

        [[nodiscard]] const Acceleration &acceleration() const { return _acceleration; }

        Acceleration &acceleration() { return _acceleration; }

        [[nodiscard]] const Quadrupole &quadrupole() const { return _quadrupole; }

        Quadrupole &quadrupole() { return _quadrupole; }

        void translate(const glm::vec3 &offset) {
            _acceleration += offset;
            // todo
        }

    };
}

#endif //N_BODY_QUADRUPOLEACCELERATION_H
