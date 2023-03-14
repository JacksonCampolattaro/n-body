//
// Created by Jackson Campolattaro on 3/2/23.
//

#ifndef N_BODY_MULTIPOLEMOMENT_H
#define N_BODY_MULTIPOLEMOMENT_H

#include <NBody/Physics/Multipole.h>
#include <NBody/Physics/Position.h>
#include <NBody/Physics/Mass.h>

namespace NBody::Physics {

    template<std::size_t Order>
    class MultipoleMoment : public Multipole<Order> {
    public:

        MultipoleMoment() = default;

        explicit MultipoleMoment(const glm::vec3 &offset) : Multipole<Order>() {
            init(offset);
        }

        using Multipole<Order>::tensors;
        using Multipole<Order>::tensor;
        using Multipole<Order>::get;

        MultipoleMoment<Order> traceless() const {
            MultipoleMoment<Order> copy;
            [&]<std::size_t... Orders>(std::index_sequence<Orders...>) {
                ((std::get<Orders + 1>(copy.tensors()) = std::get<Orders + 1>(tensors()).traceless()), ...);
            }(std::make_index_sequence<Order - 1>());
            return copy;
        };

        void enforceTraceless() {
            [&]<std::size_t... Orders>(std::index_sequence<Orders...>) {
                ((std::get<Orders + 1>(tensors()).enforceTraceless()), ...);
            }(std::make_index_sequence<Order - 1>());
        };

        MultipoleMoment<Order> &operator*=(const Mass &rhs) {
            Multipole<Order>::operator*=(rhs.mass());
            return *this;
        };

        template<std::size_t TensorOrder = Order>
        inline void applyCoefficients() {
            if constexpr (TensorOrder == 0) return;
            else {
                Multipole<Order>::template tensor<TensorOrder>() *= coefficient<TensorOrder>();
                applyCoefficients<TensorOrder - 1>();
            }
        }

    private:

        template<std::size_t TensorOrder = Order>
        void init(const glm::vec3 &offset) {
            if constexpr (TensorOrder == 2)
                Multipole<Order>::template tensor<2>() =
                        SymmetricTensor3<2>::outerProduct(offset, offset).traceless() * coefficient<2>();
            else {

                // Recursively find all the lower order tensors
                init<TensorOrder - 1>(offset);

                // The new tensor is produced by the outer product of the second-highest tensor with itself
                Multipole<Order>::template tensor<TensorOrder>() = SymmetricTensor3<TensorOrder>::outerProduct(
                        Multipole<Order>::template tensor<TensorOrder - 1>(),
                        Multipole<Order>::template tensor<TensorOrder - 1>()
                ).traceless() * coefficient<TensorOrder>();

            }
        }


        template<std::size_t TensorOrder>
        constexpr std::size_t coefficient() {
            // fixme: signs must be consistent with those assumed by rule()
            constexpr std::array coefficients{1, 3, 15, -105, 945}; // todo: extend this list
            return coefficients[TensorOrder - 1];
        }

    };

    template<std::size_t Order>
    static MultipoleMoment<Order> operator*(const MultipoleMoment<Order> &lhs, const Mass &rhs) {
        MultipoleMoment<Order> result = lhs;
        result *= rhs;
        return result;
    }

    template<std::size_t Order>
    static MultipoleMoment<Order> operator/(const MultipoleMoment<Order> &lhs, const Mass &rhs) {
        MultipoleMoment<Order> result = lhs;
        result /= rhs.mass();
        return result;
    }

    template<std::size_t Order>
    static MultipoleMoment<Order> operator+(const MultipoleMoment<Order> &lhs, const MultipoleMoment<Order> &rhs) {
        MultipoleMoment<Order> result = lhs;
        result += rhs;
        return result;
    }
}

#endif //N_BODY_MULTIPOLEMOMENT_H
