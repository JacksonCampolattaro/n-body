//
// Created by Jackson Campolattaro on 3/2/23.
//

#ifndef N_BODY_MULTIPOLEACCELERATION_H
#define N_BODY_MULTIPOLEACCELERATION_H

#include <NBody/Physics/Multipole.h>

namespace NBody::Physics {

    template<std::size_t Order>
    class MultipoleAcceleration : public Multipole<Order> {
    protected:

        Acceleration _acceleration{};

    public:

        MultipoleAcceleration() = default;

        template<typename... Args>
        explicit MultipoleAcceleration(const Acceleration &acceleration, Args &&... args) :
                _acceleration(acceleration), Multipole<Order>(std::forward_as_tuple(args...)) {}

        using Multipole<Order>::tensor;

        [[nodiscard]] Acceleration &vector() { return _acceleration; }

        [[nodiscard]] const Acceleration &vector() const { return _acceleration; }

        [[nodiscard]] Acceleration at(const glm::vec3 &offset) const {
            // todo: generalize to higher orders
            return _acceleration - (Multipole<Order>::template tensor<2>() * offset);
        }

        MultipoleAcceleration<Order> translated(const glm::vec3 &offset) const {
            // The quadrupole stays the same, but the local acceleration is adjusted based on the offset
            // todo: generalize to higher orders
            return MultipoleAcceleration<Order>{at(offset), Multipole<Order>::template tensor<2>()};
        }
    };
}

#endif //N_BODY_MULTIPOLEACCELERATION_H
