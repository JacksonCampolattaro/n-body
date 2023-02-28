//
// Created by Jackson Campolattaro on 2/28/23.
//

#ifndef N_BODY_MULTIPOLE_H
#define N_BODY_MULTIPOLE_H

#include <NBody/Physics/SymmetricMatrix3.h>

#include <tuple>
#include <utility>

namespace NBody {

    template<std::size_t Order>
    class Multipole {
    public:

        using SymmetricMatrices = decltype(std::tuple_cat(
                std::declval<typename Multipole<Order - 1>::SymmetricMatrices>(),
                std::declval<std::tuple<SymmetricMatrix3<Order>>>()
        ));

    private:

        SymmetricMatrices _matrices;

    public:

        template<typename... Args>
        explicit Multipole(Args&&... args) : _matrices(std::forward<Args>(args)...) {}

        template<std::size_t MatrixOrder>
        auto &matrix() {
            return std::get<MatrixOrder-1>(_matrices);
        }

        template<std::size_t MatrixOrder>
        const auto &matrix() const {
            return std::get<MatrixOrder-1>(_matrices);
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
        using SymmetricMatrices = std::tuple<>;
    };
}

#endif //N_BODY_MULTIPOLE_H
