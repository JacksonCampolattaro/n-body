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
            auto a = _acceleration;

            // todo: generalize to higher orders
            if constexpr (Order >= 2)
                a += (Multipole<Order>::template tensor<2>() * offset);
            if constexpr (Order >= 3)
                a += fullyContract(Multipole<Order>::template tensor<3>(), offset) / 2.0f;
            if constexpr (Order >= 4)
                a += fullyContract(Multipole<Order>::template tensor<4>(), offset) / 6.0f;
            if constexpr (Order >= 5)
                a += fullyContract(Multipole<Order>::template tensor<5>(), offset) / 24.0f;
            if constexpr (Order >= 6)
                a += fullyContract(Multipole<Order>::template tensor<6>(), offset) / 120.0f;

            return a;
        }

        MultipoleAcceleration<Order> translated(const glm::vec3 &offset) const {
            MultipoleAcceleration<Order> t{*this};

            if constexpr (Order >= 2) {
                auto delta = Multipole<Order>::template tensor<2>() * offset;
                t.vector() += delta;
            }
            if constexpr (Order >= 3) {
                auto delta2 = Multipole<Order>::template tensor<3>() * offset;
                auto delta = delta2 * offset;
                t.template tensor<2>() += delta2;
                t.vector() += delta / 2.0f;
            }
            if constexpr (Order >= 4) {
                auto delta3 = Multipole<Order>::template tensor<4>() * offset;
                auto delta2 = delta3 * offset;
                auto delta = delta2 * offset;
                t.template tensor<3>() += delta3;
                t.template tensor<2>() += delta2 / 2.0f;
                t.vector() += delta / 6.0f;
            }
            if constexpr (Order >= 5) {
                auto delta4 = Multipole<Order>::template tensor<5>() * offset;
                auto delta3 = delta4 * offset;
                auto delta2 = delta3 * offset;
                auto delta = delta2 * offset;
                t.template tensor<4>() += delta4;
                t.template tensor<3>() += delta3 / 2.0f;
                t.template tensor<2>() += delta2 / 6.0f;
                t.vector() += delta / 24.0f;
            }

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
