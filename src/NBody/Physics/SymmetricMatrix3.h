//
// Created by Jackson Campolattaro on 2/27/23.
//

#ifndef N_BODY_SYMMETRICMATRIX3_H
#define N_BODY_SYMMETRICMATRIX3_H

#include <array>
#include <span>

#include <glm/vec3.hpp>

namespace NBody {

    // todo: is there a better place to put this?
    enum class Dimension : std::size_t {
        X = 0,
        Y = 1,
        Z = 2
    };

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

        template<Dimension... Indices>
        float &get() {
            return _data[linearIndex<Indices...>()];
        }

        template<Dimension... Indices>
        [[nodiscard]] const float &get() const {
            return _data[linearIndex<Indices...>()];
        }

        template<Dimension... Indices>
        static constexpr std::size_t linearIndex() {

            // Make sure the user passed the right number of indices
            static_assert(sizeof...(Indices) == Order, "Incorrect number of indices for multipole order");

            // Put the indices in XYZ order, to ensure we're accessing the correct hyper-pyramid of the symmetric matrix
            constexpr auto indices = asSortedArray<Indices...>();

            // Convert the array to a linear index
            return linearIndex<indices>();
        }

        template<std::array<Dimension, Order> Indices>
        static constexpr std::size_t linearIndex() {

            // If the first index is X, then we can use lower-dimensional indexing
            if (Indices[0] == Dimension::X) {
                constexpr auto copy = lowerOrderIndices<Indices>();
                return SymmetricMatrix3<Order - 1>::template linearIndex<copy>();
            }

            // All the extra linear indices will correspond to indices which contain only Ys and Zs
            // We can determine the offset by counting the Zs
            constexpr std::size_t numberOfZs = std::count(Indices.begin(), Indices.end(), Dimension::Z);
            return SymmetricMatrix3<Order - 1>::DataSize + numberOfZs;
        }

    protected:

        template<Dimension... Indices>
        static constexpr std::array<Dimension, Order> asSortedArray() {
            std::array<Dimension, Order> array{Indices...};
            std::sort(array.begin(), array.end());
            return array;
        }

        template<std::array<Dimension, Order> Indices>
        static constexpr std::array<Dimension, Order - 1> lowerOrderIndices() {
            std::array<Dimension, Order - 1> copy;
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

        template<Dimension Index>
        float &get() { return operator[](linearIndex<Index>()); }

        template<Dimension Index>
        [[nodiscard]] const float &get() const { return operator[](linearIndex<Index>()); }

        template<Dimension Index>
        static constexpr std::size_t linearIndex() {
            return linearIndex<std::array<Dimension, 1>{Index}>();
        }

        template<std::array<Dimension, 1> Indices>
        static constexpr std::size_t linearIndex() {
            static_assert((std::size_t) Indices[0] < 3, "Index out of bounds!");
            return (std::size_t) Indices[0];
        }

    };

}

#endif //N_BODY_SYMMETRICMATRIX3_H
