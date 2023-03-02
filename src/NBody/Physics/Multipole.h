//
// Created by Jackson Campolattaro on 2/28/23.
//

#ifndef N_BODY_MULTIPOLE_H
#define N_BODY_MULTIPOLE_H

#include <NBody/Physics/SymmetricTensor3.h>

#include <tuple>
#include <utility>

namespace NBody {

    template<std::size_t Order>
    class Multipole {
    public:

        using SymmetricTensors = decltype(std::tuple_cat(
                std::declval<typename Multipole<Order - 1>::SymmetricMatrices>(),
                std::declval<std::tuple<SymmetricTensor3<Order>>>()
        ));

    private:

        SymmetricTensors _tensors;

    public:

        template<typename... Args>
        explicit Multipole(Args&&... args) : _tensors(std::forward<Args>(args)...) {}

        template<std::size_t TensorOrder>
        auto &matrix() {
            return std::get<TensorOrder - 1>(_tensors);
        }

        template<std::size_t TensorOrder>
        const auto &matrix() const {
            return std::get<TensorOrder-1>(_tensors);
        }

        template<Dimension... Indices>
        float &get() {
            return matrix<sizeof...(Indices)>().template get<Indices...>();
        }

        template<Dimension... Indices>
        [[nodiscard]] const float &get() const {
            return matrix<sizeof...(Indices)>().template get<Indices...>();
        }

        bool operator==(const Multipole<Order> &) const = default;

    };

    template<>
    class Multipole<0> {
    public:
        using SymmetricTensors = std::tuple<>;
    };
}

#endif //N_BODY_MULTIPOLE_H
