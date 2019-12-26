#ifndef Magnum_Math_TypeTraits_h
#define Magnum_Math_TypeTraits_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

/** @file
 * @brief Type traits
 */

#include <Corrade/Utility/StlMath.h>

#include "Magnum/Math/Math.h"
#include "Magnum/Types.h"

/**
@brief Precision when testing floats for equality

They have "at least" 6 significant digits of precision, taking one digit less
for more headroom.
*/
#ifndef FLOAT_EQUALITY_PRECISION
#define FLOAT_EQUALITY_PRECISION 1.0e-5f
#endif

/**
@brief Precision when testing doubles for equality

They have "at least" 15 significant digits of precision, taking one digit less
for more headroom.
*/
#ifndef DOUBLE_EQUALITY_PRECISION
#define DOUBLE_EQUALITY_PRECISION 1.0e-14
#endif

#ifndef CORRADE_TARGET_EMSCRIPTEN
/**
@brief Precision when testing long doubles for equality

They have "at least" 18 significant digits of precision, taking one digit less
for more headroom.

@attention On MSVC the precision is the same as for doubles, because they are
    internally the same type (source: https://msdn.microsoft.com/en-us/library/9cx8xs15.aspx).
    The same is apparently for 32-bit @ref CORRADE_TARGET_ANDROID "Android"
    (64-bit works as expected), but I couldn't find any source for that. This
    macro (and everything else related to @cpp long double @ce types is not
    available on @ref CORRADE_TARGET_EMSCRIPTEN "Emscripten" as the max
    supported floating-point precision there is 64 bits.
*/
#ifndef LONG_DOUBLE_EQUALITY_PRECISION
#if !defined(_MSC_VER) && (!defined(CORRADE_TARGET_ANDROID) || __LP64__)
#define LONG_DOUBLE_EQUALITY_PRECISION 1.0e-17l
#else
#define LONG_DOUBLE_EQUALITY_PRECISION 1.0e-14
#endif
#endif
#endif

namespace Magnum { namespace Math {

/**
@brief Whether @p T is an arithmetic scalar type
@m_since{2019,10}

Equivalent to @ref std::true_type for all builtin scalar integer and
floating-point types and in addition also @ref Deg and @ref Rad; equivalent to
@ref std::false_type otherwise. The @ref Half type deliberately doesn't support
any arithmetic, so it's not treated as a scalar type.

Note that this is *different* from @ref std::is_scalar, which is @cpp true @ce
also for enums or pointers --- it's rather closer to @ref std::is_arithmetic,
except that it doesn't give @ref std::true_type for @cpp bool @ce. The name is
chosen particularly for the @ref IsVector / @ref IsScalar distinction.
@see @ref IsFloatingPoint, @ref IsIntegral, @ref UnderlyingTypeOf
*/
template<class T> struct IsScalar
    #ifndef DOXYGEN_GENERATING_OUTPUT
    : std::false_type
    #endif
    {};

#ifndef DOXYGEN_GENERATING_OUTPUT
/* Can't use our own typedefs because they don't cover all types (signed char
   vs char, long vs long long etc.). Funny that char != signed char but signed
   int = int */
template<> struct IsScalar<char>: std::true_type {};
template<> struct IsScalar<signed char>: std::true_type {};
template<> struct IsScalar<unsigned char>: std::true_type {};
template<> struct IsScalar<short>: std::true_type {};
template<> struct IsScalar<unsigned short>: std::true_type {};
template<> struct IsScalar<int>: std::true_type {};
template<> struct IsScalar<unsigned int>: std::true_type {};
template<> struct IsScalar<long>: std::true_type {};
template<> struct IsScalar<unsigned long>: std::true_type {};
template<> struct IsScalar<long long>: std::true_type {};
template<> struct IsScalar<unsigned long long>: std::true_type {};
template<> struct IsScalar<float>: std::true_type {};
template<> struct IsScalar<double>: std::true_type {};
#ifndef CORRADE_TARGET_EMSCRIPTEN
template<> struct IsScalar<long double>: std::true_type {};
#endif
template<template<class> class Derived, class T> struct IsScalar<Unit<Derived, T>>: std::true_type {};
template<class T> struct IsScalar<Deg<T>>: std::true_type {};
template<class T> struct IsScalar<Rad<T>>: std::true_type {};
#endif

/**
@brief Whether @p T is an arithmetic vector type
@m_since{2019,10}

Equivalent to @ref std::true_type for all @ref Vector types and their
subclasses; equivalent to @ref std::false_type otherwise. In particular, gives
@ref std::false_type for @ref BoolVector, all matrix types, @ref Complex or
@ref Quaternion.
@see @ref IsScalar, @ref IsFloatingPoint, @ref IsIntegral
*/
template<class T> struct IsVector
    #ifndef DOXYGEN_GENERATING_OUTPUT
    : std::false_type
    #endif
    {};

#ifndef DOXYGEN_GENERATING_OUTPUT
template<std::size_t size, class T> struct IsVector<Vector<size, T>>: std::true_type {};
template<class T> struct IsVector<Vector2<T>>: std::true_type {};
template<class T> struct IsVector<Vector3<T>>: std::true_type {};
template<class T> struct IsVector<Vector4<T>>: std::true_type {};
template<class T> struct IsVector<Color3<T>>: std::true_type {};
template<class T> struct IsVector<Color4<T>>: std::true_type {};
#endif

/**
@brief Whether @p T is integral
@m_since{2019,10}

Equivalent to @ref std::true_type for all integral scalar and vector types
supported by Magnum math; equivalent to @ref std::false_type otherwise.

Unlike @ref std::is_integral this is @ref std::false_type for @cpp bool @ce.
@see @ref IsFloatingPoint, @ref IsScalar, @ref IsVector
*/
template<class T> struct IsIntegral
    #ifndef DOXYGEN_GENERATING_OUTPUT
    : std::false_type
    #endif
    {};

#ifndef DOXYGEN_GENERATING_OUTPUT
/* Can't use our own typedefs because they don't cover all types (signed char
   vs char, long vs long long etc.). Funny that char != signed char but signed
   int = int */
template<> struct IsIntegral<char>: std::true_type {};
template<> struct IsIntegral<signed char>: std::true_type {};
template<> struct IsIntegral<unsigned char>: std::true_type {};
template<> struct IsIntegral<short>: std::true_type {};
template<> struct IsIntegral<unsigned short>: std::true_type {};
template<> struct IsIntegral<int>: std::true_type {};
template<> struct IsIntegral<unsigned int>: std::true_type {};
template<> struct IsIntegral<long>: std::true_type {};
template<> struct IsIntegral<unsigned long>: std::true_type {};
template<> struct IsIntegral<long long>: std::true_type {};
template<> struct IsIntegral<unsigned long long>: std::true_type {};
template<std::size_t size, class T> struct IsIntegral<Vector<size, T>>: IsIntegral<T> {};
template<class T> struct IsIntegral<Vector2<T>>: IsIntegral<T> {};
template<class T> struct IsIntegral<Vector3<T>>: IsIntegral<T> {};
template<class T> struct IsIntegral<Vector4<T>>: IsIntegral<T> {};
template<class T> struct IsIntegral<Color3<T>>: IsIntegral<T> {};
template<class T> struct IsIntegral<Color4<T>>: IsIntegral<T> {};
/* I don't expect Deg/Rad to ever have an integral base type */
#endif

/**
@brief Whether @p T is floating-point
@m_since{2019,10}

Equivalent to @ref std::true_type for all floating-point scalar and vector
types supported by Magnum math including @ref Deg and @ref Rad; equivalent to
@ref std::false_type otherwise. The @ref Half type deliberately doesn't support
any arithmetic, so it's not treated as a floating-point type.
@see @ref IsIntegral, @ref IsScalar, @ref IsVector, @ref std::is_floating_point
*/
template<class T> struct IsFloatingPoint
    #ifndef DOXYGEN_GENERATING_OUTPUT
    : std::false_type
    #endif
    {};

#ifndef DOXYGEN_GENERATING_OUTPUT
template<> struct IsFloatingPoint<Float>: std::true_type {};
template<> struct IsFloatingPoint<Double>: std::true_type {};
#ifndef CORRADE_TARGET_EMSCRIPTEN
template<> struct IsFloatingPoint<long double>: std::true_type {};
#endif
template<std::size_t size, class T> struct IsFloatingPoint<Vector<size, T>>: IsFloatingPoint<T> {};
template<class T> struct IsFloatingPoint<Vector2<T>>: IsFloatingPoint<T> {};
template<class T> struct IsFloatingPoint<Vector3<T>>: IsFloatingPoint<T> {};
template<class T> struct IsFloatingPoint<Vector4<T>>: IsFloatingPoint<T> {};
template<class T> struct IsFloatingPoint<Color3<T>>: IsFloatingPoint<T> {};
template<class T> struct IsFloatingPoint<Color4<T>>: IsFloatingPoint<T> {};
/* Deg<Half> is legal but Half is not an arithmetic type (and thus not
   floating-point), so need to check the underlying type as well */
template<template<class> class Derived, class T> struct IsFloatingPoint<Unit<Derived, T>>: IsFloatingPoint<T> {};
template<class T> struct IsFloatingPoint<Deg<T>>: IsFloatingPoint<T> {};
template<class T> struct IsFloatingPoint<Rad<T>>: IsFloatingPoint<T> {};
#endif

/**
@brief Whether @p T is a unitless type
@m_since{2019,10}

Equivalent to @ref std::true_type for scalar or vector types that have an
unitless underlying type (i.e., not @ref Deg or @ref Rad); @ref std::false_type
otherwise. Some math functions such as @ref sqrt() or @ref log() work only with
unitless types because the resulting unit couldn't be expressed otherwise.
@see @ref IsScalar, @ref IsVector
*/
template<class T> struct IsUnitless
    #ifndef DOXYGEN_GENERATING_OUTPUT
    : std::integral_constant<bool, IsScalar<T>::value || IsVector<T>::value>
    #endif
    {};
template<template<class> class Derived, class T> struct IsUnitless<Unit<Derived, T>>: std::false_type {};
template<class T> struct IsUnitless<Deg<T>>: std::false_type {};
template<class T> struct IsUnitless<Rad<T>>: std::false_type {};

namespace Implementation {
    template<class T> struct UnderlyingType {
        static_assert(IsScalar<T>::value, "type is not scalar");
        typedef T Type;
    };
    template<template<class> class Derived, class T> struct UnderlyingType<Unit<Derived, T>> {
        typedef T Type;
    };
    template<class T> struct UnderlyingType<Deg<T>> { typedef T Type; };
    template<class T> struct UnderlyingType<Rad<T>> { typedef T Type; };
    template<std::size_t size, class T> struct UnderlyingType<Vector<size, T>> {
        typedef T Type;
    };
    template<class T> struct UnderlyingType<Vector2<T>> { typedef T Type; };
    template<class T> struct UnderlyingType<Vector3<T>> { typedef T Type; };
    template<class T> struct UnderlyingType<Vector4<T>> { typedef T Type; };
    template<class T> struct UnderlyingType<Color3<T>> { typedef T Type; };
    template<class T> struct UnderlyingType<Color4<T>> { typedef T Type; };
    template<std::size_t cols, std::size_t rows, class T> struct UnderlyingType<RectangularMatrix<cols, rows, T>> {
        typedef T Type;
    };
    template<std::size_t size, class T> struct UnderlyingType<Matrix<size, T>> {
        typedef T Type;
    };
    template<class T> struct UnderlyingType<Matrix3<T>> { typedef T Type; };
    template<class T> struct UnderlyingType<Matrix4<T>> { typedef T Type; };
}

/**
@brief Underlying type of a math type
@m_since{2019,10}

For builtin scalar types returns the type itself, for wrapped types like
@ref Deg or @ref Rad returns the underlying builtin type, for vector and matrix
types the type of their components.

For scalar types it's guaranteed that the input type is always explicitly
convertible to the output type and the output type is usable with standard APIs
such as @ref std::isinf().
*/
template<class T> using UnderlyingTypeOf = typename Implementation::UnderlyingType<T>::Type;

namespace Implementation {
    template<class T> struct TypeTraitsDefault {
        TypeTraitsDefault() = delete;

        constexpr static bool equals(T a, T b) {
            return a == b;
        }

        constexpr static bool equalsZero(T a, T) {
            return !a;
        }
    };
}

/**
@brief Traits class for builtin arithmetic types

Traits classes are usable for detecting type features at compile time without
the need for repeated code such as method overloading or template
specialization for given types. All builtin arithmetic types have this class
implemented.
*/
template<class T> struct TypeTraits: Implementation::TypeTraitsDefault<T> {
    /*
     * The following values are implemented as inline functions, not as
     * static const variables, because the compiler will inline the return
     * values instead of referencing to static data and unlike static const
     * variables the functions can be overloaded, deleted and hidden.
     */

    #ifdef DOXYGEN_GENERATING_OUTPUT
    /**
     * @brief Corresponding floating-point type for normalization
     *
     * If the type is not already floating-point, defines smallest larger
     * floating-point type.
     */
    typedef U FloatingPointType;

    /**
     * @brief Type name
     *
     * Returns a string representation of type name, such as `"UnsignedInt"`
     * for @ref Magnum::UnsignedInt "UnsignedInt".
     */
    constexpr static const char* name();

    /**
     * @brief Epsilon value for fuzzy compare
     *
     * Returns minimal difference between numbers to be considered
     * inequal. Returns 1 for integer types and reasonably small value for
     * floating-point types. Not implemented for arbitrary types.
     */
    constexpr static T epsilon();

    /**
     * @brief Fuzzy compare
     *
     * Uses fuzzy compare for floating-point types (using @ref epsilon()
     * value), pure equality comparison everywhere else. Algorithm adapted from
     * http://floating-point-gui.de/errors/comparison/.
     * @see @ref Math::equal(T, T), @ref Math::notEqual(T, T)
     */
    static bool equals(T a, T b);

    /**
     * @brief Fuzzy compare to zero with magnitude
     *
     * Uses fuzzy compare for floating-point types (using @ref epsilon()
     * value), pure equality comparison everywhere else. Use this function when
     * comparing e.g. a calculated nearly-zero difference with zero, knowing
     * the magnitude of original values so the epsilon can be properly scaled.
     * In other words, the following lines are equivalent:
     *
     * @snippet MagnumMath.cpp TypeTraits-equalsZero
     */
    static bool equalsZero(T a, T magnitude);
    #endif
};

/**
@brief Equality comparison of scalar types
@m_since{2019,10}

Calls @ref TypeTraits<T>::equals() --- using fuzzy compare for floating-point
types and doing equality comparison on integral types. Scalar complement to
@ref equal(const Vector<size, T>& a, const Vector<size, T>&).
*/
template<class T> inline typename std::enable_if<IsScalar<T>::value, bool>::type equal(T a, T b) {
    return TypeTraits<T>::equals(a, b);
}

/**
@brief Non-equality comparison of scalar types
@m_since{2019,10}

Calls @ref TypeTraits<T>::equals() --- using fuzzy compare for floating-point
types and doing equality comparison on integral types. Scalar complement to
@ref notEqual(const Vector<size, T>& a, const Vector<size, T>&).
*/
template<class T> inline typename std::enable_if<IsScalar<T>::value, bool>::type notEqual(T a, T b) {
    return !TypeTraits<T>::equals(a, b);
}

/* Integral scalar types */
namespace Implementation {
    template<class> struct TypeTraitsName;
    #ifndef DOXYGEN_GENERATING_OUTPUT
    #define _c(type) template<> struct TypeTraitsName<type> { \
        constexpr static const char* name() { return #type; } \
    };
    _c(UnsignedByte)
    _c(Byte)
    _c(UnsignedShort)
    _c(Short)
    _c(UnsignedInt)
    _c(Int)
    #ifndef CORRADE_TARGET_EMSCRIPTEN
    _c(UnsignedLong)
    _c(Long)
    #endif
    _c(Float)
    _c(Double)
    #ifndef CORRADE_TARGET_EMSCRIPTEN
    _c(long double)
    #endif
    #undef _c
    #endif

    template<class T> struct TypeTraitsIntegral: TypeTraitsDefault<T>, TypeTraitsName<T> {
        constexpr static T epsilon() { return T(1); }
    };
}

#ifndef DOXYGEN_GENERATING_OUTPUT
template<> struct TypeTraits<UnsignedByte>: Implementation::TypeTraitsIntegral<UnsignedByte> {
    typedef Float FloatingPointType;
};
template<> struct TypeTraits<Byte>: Implementation::TypeTraitsIntegral<Byte> {
    typedef Float FloatingPointType;
};
template<> struct TypeTraits<UnsignedShort>: Implementation::TypeTraitsIntegral<UnsignedShort> {
    typedef Float FloatingPointType;
};
template<> struct TypeTraits<Short>: Implementation::TypeTraitsIntegral<Short> {
    typedef Float FloatingPointType;
};
template<> struct TypeTraits<UnsignedInt>: Implementation::TypeTraitsIntegral<UnsignedInt> {
    typedef Double FloatingPointType;
};
template<> struct TypeTraits<Int>: Implementation::TypeTraitsIntegral<Int> {
    typedef Double FloatingPointType;
};
#ifndef CORRADE_TARGET_EMSCRIPTEN
template<> struct TypeTraits<UnsignedLong>: Implementation::TypeTraitsIntegral<UnsignedLong> {
    typedef long double FloatingPointType;
};
template<> struct TypeTraits<Long>: Implementation::TypeTraitsIntegral<Long> {
    typedef long double FloatingPointType;
};
#endif

/* Floating-point scalar types */
namespace Implementation {

template<class T> struct TypeTraitsFloatingPoint: TypeTraitsName<T> {
    TypeTraitsFloatingPoint() = delete;

    static bool equals(T a, T b);
    static bool equalsZero(T a, T epsilon);
};

template<class T> bool TypeTraitsFloatingPoint<T>::equals(const T a, const T b) {
    /* Shortcut for binary equality (also infinites) */
    if(a == b) return true;

    const T absA = std::abs(a);
    const T absB = std::abs(b);
    const T difference = std::abs(a - b);

    /* One of the numbers is zero or both are extremely close to it, relative
       error is meaningless */
    if(a == T{} || b == T{} || difference < TypeTraits<T>::epsilon())
        return difference < TypeTraits<T>::epsilon();

    /* Relative error */
    return difference/(absA + absB) < TypeTraits<T>::epsilon();
}

template<class T> bool TypeTraitsFloatingPoint<T>::equalsZero(const T a, const T magnitude) {
    /* Shortcut for binary equality */
    if(a == T(0.0)) return true;

    const T absA = std::abs(a);

    /* The value is extremely close to zero, relative error is meaningless */
    if(absA < TypeTraits<T>::epsilon())
        return absA < TypeTraits<T>::epsilon();

    /* Relative error */
    return absA*T(0.5)/magnitude < TypeTraits<T>::epsilon();
}

}

template<> struct TypeTraits<Float>: Implementation::TypeTraitsFloatingPoint<Float> {
    typedef Float FloatingPointType;

    constexpr static Float epsilon() { return FLOAT_EQUALITY_PRECISION; }
};
template<> struct TypeTraits<Double>: Implementation::TypeTraitsFloatingPoint<Double> {
    typedef Double FloatingPointType;

    constexpr static Double epsilon() { return DOUBLE_EQUALITY_PRECISION; }
};
#ifndef CORRADE_TARGET_EMSCRIPTEN
template<> struct TypeTraits<long double>: Implementation::TypeTraitsFloatingPoint<long double> {
    typedef long double FloatingPointType;

    constexpr static long double epsilon() { return LONG_DOUBLE_EQUALITY_PRECISION; }
};
#endif

namespace Implementation {

/* Comparing squared length to 1 is not sufficient to compare within range
   [1 - epsilon, 1 + epsilon], as e.g. Quaternion with dot() = 1 + 1e-7 when
   converted to matrix has column vectors with dot() = 1 + 1e-6, which is just
   above 1 + epsilon. Thus it's needed to compare sqrt(dot()) in range
   [1 - epsilon, 1 + epsilon] or dot() in range [1 - 2*epsilon + epsilon^2,
   1 + 2*epsilon + epsilon^2]. Because epsilon^2 is way off machine precision,
   it's omitted. */
template<class T> inline bool isNormalizedSquared(T lengthSquared) {
    return std::abs(lengthSquared - T(1)) < T(2)*TypeTraits<T>::epsilon();
}

}
#endif

}}

#endif
