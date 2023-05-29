//
// Created by Jackson Campolattaro on 2/28/23.
//

#ifndef N_BODY_MULTIPOLE_H
#define N_BODY_MULTIPOLE_H

#include <NBody/Physics/SymmetricTensor3.h>

#include <spdlog/spdlog.h>

#include <tuple>
#include <utility>

namespace NBody {

    namespace {

        template<std::size_t Order>
        struct [[maybe_unused]] SymmetricTensorTuple {
            using type = decltype(std::tuple_cat(
                    std::declval<typename SymmetricTensorTuple<Order - 1>::type>(),
                    std::declval<std::tuple<SymmetricTensor3<Order>>>()
            ));
        };

        template<>
        struct [[maybe_unused]] SymmetricTensorTuple<1> {
            using type = std::tuple<>;
        };

    }

    template<std::size_t Order>
    class Multipole {
    public:

        using SymmetricTensors = typename SymmetricTensorTuple<Order>::type;

    protected:

        SymmetricTensors _tensors;

    public:

        static std::string name() {
            switch (Order) {
                case 2: return "Quadrupole";
                case 3: return "Octupole";
                case 4: return "Hexadecupole";
                case 5: return "Triacontadyupole";
                default: return fmt::format("{}-pole", Order);
            }
        }

        Multipole() = default;

        Multipole(const Multipole<Order> &other) : _tensors(other._tensors) {}

        Multipole(const SymmetricTensors &tensors) : _tensors(tensors) {}

        [[nodiscard]] const SymmetricTensors &tensors() const { return _tensors; }

        SymmetricTensors &tensors() { return _tensors; }

        template<std::size_t TensorOrder>
        auto &tensor() {
            return std::get<TensorOrder - 2>(_tensors);
        }

        template<std::size_t TensorOrder>
        [[nodiscard]] const auto &tensor() const {
            return std::get<TensorOrder - 2>(_tensors);
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
                ((tensor<Orders + 2>() += rhs.tensor<Orders + 2>()), ...);
            }(std::make_index_sequence<Order - 1>());
            return *this;
        };

        template<ScalarType Scalar>
        Multipole<Order> &operator*=(const Scalar &rhs) {
            [&]<std::size_t... Orders>(std::index_sequence<Orders...>) {
                ((tensor<Orders + 2>() *= rhs), ...);
            }(std::make_index_sequence<Order - 1>());
            return *this;
        };

        template<ScalarType Scalar>
        Multipole<Order> &operator/=(const Scalar &rhs) {
            [&]<std::size_t... Orders>(std::index_sequence<Orders...>) {
                ((tensor<Orders + 2>() /= rhs), ...);
            }(std::make_index_sequence<Order - 1>());
            return *this;
        };

    };

    template<std::size_t Order>
    static Multipole<Order> operator+(const Multipole<Order> &lhs, const Multipole<Order> &rhs) {
        Multipole<Order> result = lhs;
        result += rhs;
        return result;
    }

    template<std::size_t Order, ScalarType Scalar>
    static Multipole<Order> operator*(const Multipole<Order> &lhs, const Scalar &rhs) {
        Multipole<Order> result = lhs;
        result *= rhs;
        return result;
    }
}

#endif //N_BODY_MULTIPOLE_H
