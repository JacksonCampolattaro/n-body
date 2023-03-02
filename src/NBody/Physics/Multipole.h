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
                std::declval<typename Multipole<Order - 1>::SymmetricTensors>(),
                std::declval<std::tuple<SymmetricTensor3<Order>>>()
        ));

    private:

        SymmetricTensors _tensors;

    public:

        Multipole() = default;

        template<typename... Args>
        Multipole(Args &&... args) : _tensors(std::forward<Args>(args)...) {}

        [[nodiscard]] const SymmetricTensors &tensors() const { return _tensors; }

        SymmetricTensors &tensors() { return _tensors; }

        template<std::size_t TensorOrder>
        auto &tensor() {
            return std::get<TensorOrder - 1>(_tensors);
        }

        template<std::size_t TensorOrder>
        [[nodiscard]] const auto &tensor() const {
            return std::get<TensorOrder - 1>(_tensors);
        }

        template<Dimension... Indices>
        float &get() {
            return tensor<sizeof...(Indices)>().template get<Indices...>();
        }

        template<Dimension... Indices>
        [[nodiscard]] const float &get() const {
            return tensor<sizeof...(Indices)>().template get<Indices...>();
        }

        bool operator==(const Multipole<Order> &) const = default;

        Multipole<Order> &operator+=(const Multipole<Order> &rhs) {
            [&]<std::size_t... Orders>(std::index_sequence<Orders...>) {
                ((tensor<Orders+1>() += rhs.tensor<Orders+1>()), ...);
            }(std::make_index_sequence<Order>());
            return *this;
        };

    };

    template<>
    class Multipole<1> {
    public:
        using SymmetricTensors = std::tuple<glm::vec3>;
    };
}

#endif //N_BODY_MULTIPOLE_H
