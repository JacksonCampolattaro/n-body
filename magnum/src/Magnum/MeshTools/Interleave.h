#ifndef Magnum_MeshTools_Interleave_h
#define Magnum_MeshTools_Interleave_h
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
 * @brief Function @ref Magnum::MeshTools::interleave(), @ref Magnum::MeshTools::interleaveInto()
 */

#include <cstring>
#include <Corrade/Containers/Array.h>
#include <Corrade/Utility/Assert.h>
#include <Corrade/Utility/TypeTraits.h>

#include "Magnum/Magnum.h"

namespace Magnum { namespace MeshTools {

namespace Implementation {

/* Attribute count, skipping gaps. If the attributes are just gaps, returns
   ~std::size_t{0}. It must be in the structure to have proper overload
   resolution (the functions would otherwise need to be de-inlined to break
   cyclic dependencies) */
struct AttributeCount {
    template<class T, class ...U> typename std::enable_if<!std::is_convertible<T, std::size_t>::value, std::size_t>::type operator()(const T& first, const U&...
        #if !defined(CORRADE_NO_ASSERT) || defined(CORRADE_GRACEFUL_ASSERT)
        next
        #endif
    ) const {
        #ifdef _MSC_VER
        #pragma warning(push)
        #pragma warning(disable:4127) /* conditional expression is constant (of course) */
        #endif
        CORRADE_ASSERT(sizeof...(next) == 0 || AttributeCount{}(next...) == first.size() || AttributeCount{}(next...) == ~std::size_t(0), "MeshTools::interleave(): attribute arrays don't have the same length, expected" << first.size() << "but got" << AttributeCount{}(next...), 0);
        #ifdef _MSC_VER
        #pragma warning(pop)
        #endif

        return first.size();
    }
    template<class T, class... U> std::size_t operator()(std::size_t, const T& first, const U&... next) const {
        return AttributeCount{}(first, next...);
    }
    constexpr std::size_t operator()(std::size_t) const { return ~std::size_t(0); }
    constexpr std::size_t operator()() const { return 0; }
};

/* If anybody has an idea how to do this better and not require C++17 if
   constexpr, please tell me. This is horrendous. */
CORRADE_HAS_TYPE(HasType, typename T::Type);
template<bool hasType> struct TypeSizeImpl;
template<> struct TypeSizeImpl<true> {
    template<class T> constexpr static std::size_t get() { return sizeof(typename T::Type); }
};
template<> struct TypeSizeImpl<false> {
    template<class T> constexpr static std::size_t get() { return sizeof(typename T::value_type); }
};
template<class T> constexpr std::size_t typeSize() {
    return TypeSizeImpl<HasType<T>::value>::template get<T>();
}

/* Stride, taking gaps into account. It must be in the structure, same reason
   as above */
struct Stride {
    template<class T, class ...U> typename std::enable_if<!std::is_convertible<T, std::size_t>::value, std::size_t>::type operator()(const T&, const U&... next) const {
        return typeSize<T>() + Stride{}(next...);
    }
    template<class... T> std::size_t operator()(std::size_t gap, const T&... next) const {
        return gap + Stride{}(next...);
    }
    constexpr std::size_t operator()() const { return 0; }
};

/* Copy data to the buffer */
template<class T> typename std::enable_if<!std::is_convertible<T, std::size_t>::value, std::size_t>::type writeOneInterleaved(std::size_t stride, char* startingOffset, const T& attributeList) {
    auto it = attributeList.begin();
    for(std::size_t i = 0; i != attributeList.size(); ++i, ++it)
        std::memcpy(startingOffset + i*stride, reinterpret_cast<const char*>(&*it), typeSize<T>());

    return typeSize<T>();
}

/* Skip gap */
constexpr std::size_t writeOneInterleaved(std::size_t, char*, std::size_t gap) { return gap; }

/* Write interleaved data */
inline void writeInterleaved(std::size_t, char*) {}
template<class T, class ...U> void writeInterleaved(std::size_t stride, char* startingOffset, const T& first, const U&... next) {
    writeInterleaved(stride, startingOffset + writeOneInterleaved(stride, startingOffset, first), next...);
}

}

/**
@brief Interleave vertex attributes

This function takes list of attribute arrays and returns them interleaved, so
data for each attribute are in continuous place in memory.

Example usage:

@snippet MagnumMeshTools-gl.cpp interleave1

It's often desirable to align data for one vertex on 32bit boundaries. To
achieve that, you can specify gaps between the attributes:

@snippet MagnumMeshTools.cpp interleave2

All gap bytes are set zero. This way vertex stride is 24 bytes, without gaps it
would be 21 bytes, causing possible performance loss.

@attention The function expects that all arrays have the same size.

@note The only requirements to attribute array type is that it must have either
    a typedef `T::Type` (in case of Corrade types such as
    @ref Corrade::Containers::ArrayView) or a typedef `T::value_type` (in case
    of STL types such as @ref std::vector or @ref std::array) or a, a forward
    iterator (to be used with range-based for) and a function `size()`
    returning count of elements.

@see @ref interleaveInto()
*/
template<class T, class ...U> Containers::Array<char> interleave(const T& first, const U&... next)
{
    /* Compute buffer size and stride */
    const std::size_t attributeCount = Implementation::AttributeCount{}(first, next...);
    const std::size_t stride = Implementation::Stride{}(first, next...);

    /* Create output buffer only if we have some attributes */
    if(attributeCount && attributeCount != ~std::size_t(0)) {
        Containers::Array<char> data{Containers::ValueInit, attributeCount*stride};
        Implementation::writeInterleaved(stride, data.begin(), first, next...);

        return data;

    /* Otherwise return nullptr */
    } else return nullptr;
}

/**
@brief Interleave vertex attributes into existing buffer

Unlike @ref interleave() this function interleaves the data into existing
buffer and leaves gaps untouched instead of zero-initializing them. This
function can thus be used for interleaving data depending on runtime
parameters.

@attention Similarly to @ref interleave(), this function expects that all
    arrays have the same size. The passed buffer must also be large enough to
    contain the interleaved data.
*/
template<class T, class ...U> void interleaveInto(Containers::ArrayView<char> buffer, const T& first, const U&... next) {
    /* Verify expected buffer size */
    #if !defined(CORRADE_NO_ASSERT) || defined(CORRADE_GRACEFUL_ASSERT)
    const std::size_t attributeCount = Implementation::AttributeCount{}(first, next...);
    #endif
    const std::size_t stride = Implementation::Stride{}(first, next...);
    CORRADE_ASSERT(attributeCount*stride <= buffer.size(), "MeshTools::interleaveInto(): the data buffer is too small, expected" << attributeCount*stride << "but got" << buffer.size(), );

    /* Write data */
    Implementation::writeInterleaved(stride, buffer.begin(), first, next...);
}

}}

#endif
