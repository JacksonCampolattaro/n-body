//
// Created by Jackson Campolattaro on 2/27/23.
//

#ifndef N_BODY_SYMMETRICTENSOR3_H
#define N_BODY_SYMMETRICTENSOR3_H

#include <array>
#include <span>

#include <glm/vec3.hpp>

#include <iostream>

namespace NBody {

    // todo: is there a better place to put this?
    template<typename T>
    concept ScalarType = std::is_scalar_v<T>;

    // todo: is there a better place to put this?
    enum class Dimension : std::size_t {
        X = 0,
        Y = 1,
        Z = 2
    };

    namespace {

        template<std::size_t Order>
        constexpr std::size_t valuesInMatrixOrder() {
            if constexpr (Order == 1) return 3;
            else return valuesInMatrixOrder<Order - 1>() * 3;
        }

        template<std::size_t Order>
        constexpr std::size_t uniqueValuesInMatrixOrder() {
            if constexpr (Order == 1) return 3;
            else return uniqueValuesInMatrixOrder<Order - 1>() + Order + 1;
        }

    }

    template<std::size_t Order>
    class SymmetricTensor3 {
    public:

        static constexpr std::size_t NumValues = valuesInMatrixOrder<Order>();
        static constexpr std::size_t NumUniqueValues = uniqueValuesInMatrixOrder<Order>();

        using
        enum Dimension;

    private:

        std::array<float, NumUniqueValues> _data{0};

    public: // Constructors

        SymmetricTensor3() :
                _data({0}) {}

        explicit SymmetricTensor3(std::array<float, NumUniqueValues> values) :
                _data(values) {}

        static SymmetricTensor3<Order> identity() {

            SymmetricTensor3<Order> matrix{};
            [&]<std::size_t... I>(std::index_sequence<I...>) {
                ((matrix._data[I] = kroneckerDelta<dimensionalIndex<I>()>()), ...);
            }(std::make_index_sequence<NumUniqueValues>());

            return matrix;
        }

        static SymmetricTensor3<Order> ones() {

            SymmetricTensor3<Order> matrix{};
            [&]<std::size_t... I>(std::index_sequence<I...>) {
                ((matrix._data[I] = 1.0f), ...);
            }(std::make_index_sequence<NumUniqueValues>());

            return matrix;
        }

        static SymmetricTensor3<Order> diagonal(glm::vec3 vec) {

            SymmetricTensor3<Order> tensor;
            tensor.flat()[0] = vec.x;
            tensor.flat()[SymmetricTensor3<Order - 1>::NumUniqueValues] = vec.y;
            tensor.flat()[SymmetricTensor3<Order>::NumUniqueValues - 1] = vec.z;
            return tensor;
        }

        template<typename Func>
        static SymmetricTensor3<Order> nullary(Func &&f) {

            SymmetricTensor3<Order> matrix{};
            [&]<std::size_t... I>(std::index_sequence<I...>) {
                ((matrix._data[I] = f(dimensionalIndex<I>())), ...);
            }(std::make_index_sequence<NumUniqueValues>());

            return matrix;
        }

        static SymmetricTensor3<Order> cartesianPower(const glm::vec3 &vec) {

            SymmetricTensor3<Order> matrix{};
            [&]<std::size_t... I>(std::index_sequence<I...>) {
                ((matrix._data[I] = productOfDimensions<dimensionalIndex<I>()>(vec)), ...);
            }(std::make_index_sequence<NumUniqueValues>());

            return matrix;
        }

    public: // Member Access

        template<Dimension... Indices>
        float &get() {
            return _data[linearIndex<Indices...>()];
        }

        template<Dimension... Indices>
        [[nodiscard]] const float &get() const {
            return _data[linearIndex<Indices...>()];
        }

        template<std::array<Dimension, Order> Indices>
        float &get() {
            return _data[linearIndex<Indices>()];
        }

        template<std::array<Dimension, Order> Indices>
        [[nodiscard]] const float &get() const {
            return _data[linearIndex<Indices>()];
        }

        [[nodiscard]] const std::array<float, NumUniqueValues> &flat() const { return _data; }

        std::array<float, NumUniqueValues> &flat() { return _data; }

    public: // Tensor properties

        [[nodiscard]] float trace() const {
            // The diagonal only has 3 elements, no matter how many dimensions are matrix has!
            return _data[0] + // first value is XXX...X
                   _data[SymmetricTensor3<Order - 1>::NumUniqueValues] + // first value after lower dim is YYY...Y
                   _data[NumUniqueValues - 1]; // last value is ZZZ...Z
        }

        [[nodiscard]] float sum() const {
            return [&]<std::size_t... I>(std::index_sequence<I...>) {
                return ((get<lexicographicalIndex<I>()>()) + ...);
            }(std::make_index_sequence<NumValues>());
        }

    public: // Transformations

        [[nodiscard]] SymmetricTensor3<Order> traceless() const {
            return *this - (identity() * (trace() / 3.0f));
        }

        void enforceTraceless() {
            // ensure ZZ...Z = -(XX...X + YY...Y)
            _data[NumUniqueValues - 1] = -(_data[0] + _data[SymmetricTensor3<Order - 1>::NumUniqueValues]);
        }

    public: // Unary Operators

        bool operator==(const SymmetricTensor3<Order> &) const = default;

        SymmetricTensor3<Order> &operator+=(const SymmetricTensor3<Order> &rhs) {
            for (int i = 0; i < NumUniqueValues; ++i)
                flat()[i] += rhs.flat()[i];
            return *this;
        }

        SymmetricTensor3<Order> &operator-=(const SymmetricTensor3<Order> &rhs) {
            for (int i = 0; i < NumUniqueValues; ++i)
                flat()[i] -= rhs.flat()[i];
            return *this;
        }

        template<ScalarType Scalar>
        SymmetricTensor3<Order> &operator*=(const Scalar &rhs) {
            for (int i = 0; i < NumUniqueValues; ++i)
                flat()[i] *= rhs;
            return *this;
        }

        template<ScalarType Scalar>
        SymmetricTensor3<Order> &operator/=(const Scalar &rhs) {
            for (int i = 0; i < NumUniqueValues; ++i)
                flat()[i] /= rhs;
            return *this;
        }

    public: // Binary Operators

        static SymmetricTensor3<Order> sumOfOuterProducts(const SymmetricTensor3<Order - 1> &lhs,
                                                          const glm::vec3 &rhs) {
            // todo: check that this is correct
            SymmetricTensor3<Order> matrix{};
            [&]<std::size_t... I>(std::index_sequence<I...>) {
                ((matrix.get<dimensionalIndex<I>()>() = sumOfProductsAtIndex<dimensionalIndex<I>()>(lhs, rhs)), ...);
            }(std::make_index_sequence<NumUniqueValues>());
            return matrix;
        }

        friend SymmetricTensor3<Order> operator+(const SymmetricTensor3<Order> &lhs,
                                                 const SymmetricTensor3<Order> &rhs) {
            SymmetricTensor3<Order> sum{};
            for (int i = 0; i < NumUniqueValues; ++i)
                sum.flat()[i] = lhs.flat()[i] + rhs.flat()[i];
            return sum;
        }

        friend SymmetricTensor3<Order> operator-(const SymmetricTensor3<Order> &lhs,
                                                 const SymmetricTensor3<Order> &rhs) {
            SymmetricTensor3<Order> difference{};
            for (int i = 0; i < NumUniqueValues; ++i)
                difference.flat()[i] = lhs.flat()[i] - rhs.flat()[i];
            return difference;
        }


        friend glm::vec3 operator*(const SymmetricTensor3<Order> &lhs,
                                   const SymmetricTensor3<Order - 1> &rhs) {
            glm::vec3 product{};
            [&]<std::size_t... I>(std::index_sequence<I...>) {
                ((
                        product[(std::size_t) lexicographicalIndex<I>()[0]] +=
                                lhs.template get<(lexicographicalIndex<I>())>() *
                                rhs.template get<(tail<lexicographicalIndex<I>()>())>()
                ), ...);
            }(std::make_index_sequence<NumValues>());
            return product;
        }

        friend float operator*(const SymmetricTensor3<Order> &lhs, const SymmetricTensor3<Order> &rhs) {
            SymmetricTensor3<Order> elementWiseProduct{};
            for (int i = 0; i < NumUniqueValues; ++i)
                elementWiseProduct.flat()[i] = lhs.flat()[i] * rhs.flat()[i];
            return elementWiseProduct.sum();
        }

        friend SymmetricTensor3<Order> operator*(const SymmetricTensor3<Order> &lhs, const float &rhs) {
            SymmetricTensor3<Order> product{};
            for (int i = 0; i < SymmetricTensor3<Order>::NumUniqueValues; ++i)
                product.flat()[i] = lhs.flat()[i] * rhs;
            return product;
        }

        friend SymmetricTensor3<Order> operator*(const float &lhs, const SymmetricTensor3<Order> &rhs) {
            return (rhs * lhs);
        }

        friend SymmetricTensor3<Order> operator/(const SymmetricTensor3<Order> &lhs, const float &rhs) {
            return (lhs * (1 / rhs));
        }

        // todo: this could be generalized to arbitrary lower-order tensors
        friend SymmetricTensor3<Order + 1> outerProduct(const SymmetricTensor3<Order> &lhs,
                                                        const SymmetricTensor3<Order> &rhs) {
            return SymmetricTensor3<Order + 1>::outerProduct(lhs, rhs);
        }

        friend SymmetricTensor3<Order + 1> outerProduct(const SymmetricTensor3<Order> &lhs,
                                                        const glm::vec3 &rhs) {
            return SymmetricTensor3<Order + 1>::outerProduct(lhs, rhs);
        }

        friend SymmetricTensor3<Order + 1> outerProduct(const glm::vec3 &lhs,
                                                        const SymmetricTensor3<Order> &rhs) {
            return outerProduct(rhs, lhs);
        }


        static SymmetricTensor3<Order> outerProduct(const SymmetricTensor3<Order - 1> &lhs,
                                                    const SymmetricTensor3<Order - 1> &rhs) {

            // todo: this is incorrect as an outer product, but maybe useful elsewhere?
            SymmetricTensor3<Order> matrix{};
            [&]<std::size_t... I>(std::index_sequence<I...>) {
                ((
                        matrix.get<lexicographicalIndex<I>()>() =
                                lhs.template get<(head<lexicographicalIndex<I>()>())>() *
                                rhs.template get<(tail<lexicographicalIndex<I>()>())>()
                ), ...);
            }(std::make_index_sequence<NumValues>());
            return matrix;
        }

        static SymmetricTensor3<Order> outerProduct(const SymmetricTensor3<Order - 1> &lhs,
                                                    const glm::vec3 &rhs) {
            // todo
            SymmetricTensor3<Order> matrix{};
            [&]<std::size_t... I>(std::index_sequence<I...>) {
                ((matrix.get<dimensionalIndex<I>()>() = productAtIndex<dimensionalIndex<I>()>(lhs, rhs)), ...);
            }(std::make_index_sequence<NumUniqueValues>());
            return matrix;
        }

    public: // Indexing helper methods

        template<Dimension... Indices>
        static constexpr std::size_t linearIndex() {

            // Make sure the user passed the right number of indices
            static_assert(sizeof...(Indices) == Order, "Incorrect number of indices for multipole order");

            return linearIndex<asArray<Indices...>()>();
        }

        template<std::array<Dimension, Order> Indices>
        static constexpr std::size_t linearIndex() {

            // Make certain the indices are sorted
            constexpr auto indices = sorted<Indices>();

            // If the first index is X, then we can defer to lower-dimensional indexing
            if (indices[0] == Dimension::X) {
                return SymmetricTensor3<Order - 1>::template linearIndex<tail<indices>()>();
            }

            // All the extra linear indices will correspond to indices which contain only Ys and Zs
            // We can determine the offset by counting the Zs
            constexpr std::size_t numberOfZs = std::count(indices.begin(), indices.end(), Dimension::Z);
            return SymmetricTensor3<Order - 1>::NumUniqueValues + numberOfZs;
        }

        template<std::size_t LinearIndex>
        static constexpr std::array<Dimension, Order> dimensionalIndex() {
            static_assert(LinearIndex < NumUniqueValues, "Linear index is out of bounds");

            std::array<Dimension, Order> array{};
            if constexpr (LinearIndex < SymmetricTensor3<Order - 1>::NumUniqueValues) {

                // If the linear index would fit in a smaller matrix, prefix with an X and recursively find the rest
                constexpr auto lowerIndex = SymmetricTensor3<Order - 1>::template dimensionalIndex<LinearIndex>();
                array[0] = Dimension::X;
                std::copy(lowerIndex.begin(), lowerIndex.end(), array.begin() + 1);

            } else {

                // Otherwise, the dimensional matrix is made up of Ys and Zs in appropriate proportions
                constexpr std::size_t numberOfZs =
                        LinearIndex - SymmetricTensor3<Order - 1>::NumUniqueValues;
                constexpr std::size_t numberOfYs =
                        (NumUniqueValues - SymmetricTensor3<Order - 1>::NumUniqueValues) - numberOfZs - 1;
                std::fill(array.begin(), array.begin() + numberOfYs, Dimension::Y);
                std::fill(array.begin() + numberOfYs, array.end(), Dimension::Z);
            }

            return array;
        }

        template<std::size_t LinearIndex>
        static constexpr std::array<Dimension, Order> lexicographicalIndex() {
            static_assert(LinearIndex < NumValues, "Lexicographical index is out of bounds");

            std::array<Dimension, Order> dimensions{};

            constexpr std::size_t topIndex = LinearIndex % 3;
            constexpr std::size_t lowerLinearIndex = (LinearIndex - topIndex) / 3;

            dimensions[Order - 1] = (Dimension) topIndex;
            auto lowerDimensions = SymmetricTensor3<Order - 1>::template lexicographicalIndex<lowerLinearIndex>();
            std::copy(lowerDimensions.begin(), lowerDimensions.end(), dimensions.begin());

            return dimensions;
        }

    protected: // Array operation helper methods

        template<Dimension... Indices>
        static constexpr std::array<Dimension, Order> asArray() {
            return std::array<Dimension, Order>{Indices...};
        }

        template<std::array<Dimension, Order> Unsorted>
        static constexpr std::array<Dimension, Order> sorted() {
            std::array<Dimension, Order> array = Unsorted;
            std::sort(array.begin(), array.end());
            return array;
        }

        template<std::array<Dimension, Order> Array, std::size_t N>
        static constexpr std::array<Dimension, Order> rotated() {
            std::array<Dimension, Order> copy = Array;
            std::rotate(copy.begin(), copy.begin() + N, copy.end());
            return copy;
        }

        template<std::array<Dimension, Order> Array, std::size_t N = Order - 1>
        static constexpr std::array<Dimension, N> head() {
            std::array<Dimension, N> copy{};
            std::copy(Array.begin(), Array.begin() + N, copy.begin());
            return copy;
        }

        template<std::array<Dimension, Order> Array, std::size_t N = Order - 1>
        static constexpr std::array<Dimension, N> tail() {
            std::array<Dimension, N> copy{};
            std::copy(Array.end() - N, Array.end(), copy.begin());
            return copy;
        }

    protected: // Helper methods for math operations

        template<std::array<Dimension, Order> Dimensions>
        static constexpr float productOfDimensions(const glm::vec3 &vec) {
            return []<std::size_t... I>(std::index_sequence<I...>, const glm::vec3 &vec) {
                return ((vec[(std::size_t) Dimensions[I]]) * ...);
            }(std::make_index_sequence<Order>(), vec);
        };

        template<std::array<Dimension, Order> Index>
        static constexpr float productAtIndex(const SymmetricTensor3<Order - 1> &lhs, const glm::vec3 &rhs) {
            return lhs.template get<head<Index>()>() * rhs[(std::size_t) Index[Order - 1]];
        };

        template<std::array<Dimension, Order> Index>
        static constexpr float sumOfProductsAtIndex(const SymmetricTensor3<Order - 1> &lhs, const glm::vec3 &rhs) {
            return [&]<std::size_t... I>(std::index_sequence<I...>) {
                return ((productAtIndex<rotated<Index, I>()>(lhs, rhs)) + ...);
            }(std::make_index_sequence<Order>());
        };

        template<std::array<Dimension, Order> Dimensions>
        static constexpr float kroneckerDelta() {
            return []<std::size_t... I>(std::index_sequence<I...>) {
                return (((std::size_t) Dimensions[I]) == ...) ? 1.0f : 0.0f;
            }(std::make_index_sequence<Order>());
        };

    };

    template<>
    class SymmetricTensor3<1> {
    public:

        static constexpr std::size_t NumValues = 3;
        static constexpr std::size_t NumUniqueValues = 3;

        SymmetricTensor3<1>() = delete;

        template<Dimension Index>
        static constexpr std::size_t linearIndex() {
            return linearIndex<std::array<Dimension, 1>{Index}>();
        }

        template<std::array<Dimension, 1> Indices>
        static constexpr std::size_t linearIndex() {
            static_assert((std::size_t) Indices[0] < 3, "Index out of bounds!");
            return (std::size_t) Indices[0];
        }

        template<std::size_t LinearIndex>
        static constexpr std::array<Dimension, 1> dimensionalIndex() {
            static_assert(LinearIndex < NumUniqueValues, "Linear index is out of bounds");
            return {static_cast<Dimension>(LinearIndex)};
        }

        template<std::size_t LinearIndex>
        static constexpr std::array<Dimension, 1> lexicographicalIndex() {
            static_assert(LinearIndex < NumValues, "Lexicographical index is out of bounds");
            return {(Dimension) LinearIndex};
        }

    };

    static SymmetricTensor3<2> outerProduct(const glm::vec3 &lhs,
                                            const glm::vec3 &rhs) {
        return SymmetricTensor3<2>{std::array<float, 6>{lhs.x * rhs.x, lhs.x * rhs.y, lhs.x * rhs.z,
                                                        lhs.y * rhs.y, lhs.y * rhs.z,
                                                        lhs.z * rhs.z}};
    }

    static glm::vec3 operator*(const SymmetricTensor3<2> &lhs, const glm::vec3 &rhs) {

        using
        enum Dimension;
        return {
                (lhs.get<X, X>() * rhs.x) + (lhs.get<X, Y>() * rhs.y) + (lhs.get<X, Z>() * rhs.z),
                (lhs.get<Y, X>() * rhs.x) + (lhs.get<Y, Y>() * rhs.y) + (lhs.get<Y, Z>() * rhs.z),
                (lhs.get<Z, X>() * rhs.x) + (lhs.get<Z, Y>() * rhs.y) + (lhs.get<Z, Z>() * rhs.z)
        };
    }

    static glm::vec3 operator*(const glm::vec3 &lhs, const SymmetricTensor3<2> &rhs) {
        // todo: is tensor contraction really commutative like this?
        return operator*(rhs, lhs);
    }

}

#endif //N_BODY_SYMMETRICTENSOR3_H
