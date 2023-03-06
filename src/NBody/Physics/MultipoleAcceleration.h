//
// Created by Jackson Campolattaro on 3/2/23.
//

#ifndef N_BODY_MULTIPOLEACCELERATION_H
#define N_BODY_MULTIPOLEACCELERATION_H

#include <NBody/Physics/Multipole.h>

namespace NBody::Physics {

    template<std::size_t Order>
    class MultipoleAcceleration : public Multipole<Order> {
    public:

        using Multipole<Order>::Multipole;
        using Multipole<Order>::tensor;
        using Multipole<Order>::operator==;
        using Multipole<Order>::operator+=;

        // todo: this should be removed
        [[nodiscard]] Acceleration acceleration() const {
            return Acceleration{Multipole<Order>::template tensor<1>()};
        }

        [[nodiscard]] Acceleration at(const glm::vec3 &offset) const {
            // todo: generalize to higher orders
            return Multipole<Order>::template tensor<1>() - (Multipole<Order>::template tensor<2>() * offset);
        }

        MultipoleAcceleration<Order> translated(const glm::vec3 &offset) const {
            // The quadrupole stays the same, but the local acceleration is adjusted based on the offset
            // todo: generalize to higher orders
            return {{at(offset), Multipole<Order>::template tensor<2>()}};
        }

        // todo
    };
}

#endif //N_BODY_MULTIPOLEACCELERATION_H
