//
// Created by Jackson Campolattaro on 3/2/23.
//

#ifndef N_BODY_MULTIPOLEACCELERATION_H
#define N_BODY_MULTIPOLEACCELERATION_H

#include <NBody/Physics/Multipole.h>
#include <glm/gtx/string_cast.hpp>

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
            auto a = _acceleration;

            if constexpr (Order >= 2)
                a -= (Multipole<Order>::template tensor<2>() * offset);
            if constexpr (Order >= 3) // todo: is this equivalent to repeated contraction with the vector?
                a -= (Multipole<Order>::template tensor<3>() * SymmetricTensor3<2>::cartesianPower(offset)) / 2.0f;
            if constexpr (Order >= 4) // todo: why does this sign need to be flipped?
                a -= (Multipole<Order>::template tensor<4>() * SymmetricTensor3<3>::cartesianPower(offset)) / 6.0f;

            return a;
        }

        MultipoleAcceleration<Order> translated(const glm::vec3 &offset) const {
            // The quadrupole stays the same, but the local acceleration is adjusted based on the offset
            MultipoleAcceleration<Order> t{*this};
            t._acceleration = at(offset);
            return t;
        }

        MultipoleAcceleration<Order> &operator+=(const MultipoleAcceleration<Order> &rhs) {
            _acceleration += rhs.vector();
            Multipole<Order>::operator+=(rhs);
            return *this;
        };

        friend std::ostream &operator<<(std::ostream &out, const MultipoleAcceleration<Order> &m) {
            return out << glm::to_string((glm::vec3) m.vector());
        }
    };
}

#endif //N_BODY_MULTIPOLEACCELERATION_H
