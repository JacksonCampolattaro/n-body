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
    class MultipoleMoment : Multipole<Order> {
    public:

        template<std::size_t TensorOrder = Order>
        void insert(const Position &position, const Mass &mass) {

            // todo

            // Base case
            if constexpr (TensorOrder == 0) {

                return;
            }

        }

    private:

        template<std::size_t TensorOrder>
        constexpr std::size_t coefficient() {
            constexpr std::array coefficients{-1, 3, -15, 105, -945}; // todo
            return coefficients[TensorOrder];
        }

    };

}

#endif //N_BODY_MULTIPOLEMOMENT_H
