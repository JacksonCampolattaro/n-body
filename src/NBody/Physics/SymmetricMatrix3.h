//
// Created by Jackson Campolattaro on 2/27/23.
//

#ifndef N_BODY_SYMMETRICMATRIX3_H
#define N_BODY_SYMMETRICMATRIX3_H

#include <array>
#include <span>

#include <glm/vec3.hpp>

namespace NBody {

    template<int Order>
    class SymmetricMatrix3 {
    public:

        static constexpr std::size_t DataSize = SymmetricMatrix3<Order - 1>::DataSize + (Order + 1);

    private:

        std::array<float, DataSize> _data;

    public:

        SymmetricMatrix3() :
                _data({0}) {}

        explicit SymmetricMatrix3(std::array<float, DataSize> values) :
                _data(values) {}

        template<std::size_t... Indices>
        float &get() {
            return _data[linearIndex<Indices...>()];
        }

        template<std::size_t... Indices>
        [[nodiscard]] const float &get() const {
            return _data[linearIndex<Indices...>()];
        }

        template<std::size_t... Indices>
        static constexpr std::size_t linearIndex() {

            // Make sure the user passed the right number of indices
            static_assert(sizeof...(Indices) == Order, "Incorrect number of indices for multipole order");

            // Put the indices in XYZ order, to ensure we're accessing the correct hyper-pyramid of the symmetric matrix
            constexpr auto indices = asSortedArray<Indices...>();

            // Convert the array to a linear index
            return linearIndex<indices>();
        }

        template<std::array<std::size_t, Order> Indices>
        static constexpr std::size_t linearIndex() {

            // If the first index is X, then we can use lower-dimensional indexing
            if (Indices[0] == 0) {
                constexpr auto copy = lowerOrderIndices<Indices>();
                return SymmetricMatrix3<Order - 1>::template linearIndex<copy>();
            }

            // All the extra linear indices will correspond to indices which contain only Ys and Zs
            // We can determine the offset by counting the Zs
            constexpr std::size_t numberOfZs = std::count(Indices.begin(), Indices.end(), 2);
            return SymmetricMatrix3<Order - 1>::DataSize + numberOfZs;
        }

    protected:

        template<std::size_t... Indices>
        static constexpr std::array<std::size_t, Order> asSortedArray() {
            std::array<std::size_t, Order> array{Indices...};
            std::sort(array.begin(), array.end());
            return array;
        }

        template<std::array<std::size_t, Order> Indices>
        static constexpr std::array<std::size_t, Order - 1> lowerOrderIndices() {
            std::array<std::size_t, Order - 1> copy;
            std::copy(Indices.begin() + 1, Indices.end(), copy.begin());
            return copy;
        }
    };

    template<>
    class SymmetricMatrix3<1> : public glm::vec3 {
    public:

        using glm::vec3::vec;

        static constexpr std::size_t DataSize = 3;

        SymmetricMatrix3(std::array<float, DataSize> values) : glm::vec3(values[0], values[1], values[2]) {}

        template<std::array<std::size_t, 1> Indices>
        float &get() { return operator[](Indices[0]); }

        template<std::array<std::size_t, 1> Indices>
        [[nodiscard]] const float &get() const { return operator[](Indices[0]); }

        template<std::size_t Index>
        static constexpr std::size_t linearIndex() {
            return linearIndex<std::array<std::size_t, 1>{Index}>();
        }

        template<std::array<std::size_t, 1> Indices>
        static constexpr std::size_t linearIndex() {
            static_assert(Indices[0] < 3, "Index out of bounds!");
            return Indices[0];
        }

    };

}

#endif //N_BODY_SYMMETRICMATRIX3_H
