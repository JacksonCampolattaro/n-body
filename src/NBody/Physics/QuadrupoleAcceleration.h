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

        QuadrupoleAcceleration(const Acceleration &acceleration, const Quadrupole &quadrupole) :
                _acceleration(acceleration), _quadrupole(quadrupole) {};

        [[nodiscard]] const Acceleration &acceleration() const { return _acceleration; }

        Acceleration &acceleration() { return _acceleration; }

        [[nodiscard]] const Quadrupole &quadrupole() const { return _quadrupole; }

        Quadrupole &quadrupole() { return _quadrupole; }

        QuadrupoleAcceleration &operator+=(const QuadrupoleAcceleration &right) {
            _acceleration += right.acceleration();
            _quadrupole += right.quadrupole();
            return *this;
        }

        Acceleration at(const glm::vec3 &offset) const {
            return _acceleration - (_quadrupole * offset);
        }

        QuadrupoleAcceleration translated(const glm::vec3 &offset) const {
            // The quadrupole stays the same, but the local acceleration is adjusted based on the offset
            return {at(offset), _quadrupole};
        }

        friend std::ostream &operator<<(std::ostream &out, const QuadrupoleAcceleration &v) {
            return out << fmt::format("<{}, {}, {}, ({}, {}, {}, {}, {}, {})>",
                                      v.acceleration().x, v.acceleration().y, v.acceleration().y,
                                      v.quadrupole().xx(), v.quadrupole().yy(), v._quadrupole.zz(),
                                      v.quadrupole().xy(), v.quadrupole().yz(), v.quadrupole().zx());
        }

    };
}

#endif //N_BODY_QUADRUPOLEACCELERATION_H
