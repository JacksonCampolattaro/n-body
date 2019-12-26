#ifndef Magnum_Mesh_h
#define Magnum_Mesh_h
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
 * @brief Enum @ref Magnum::MeshPrimitive, @ref Magnum::MeshIndexType, function @ref Magnum::meshIndexTypeSize()
 */

#include <Corrade/Utility/StlForwardString.h>

#include "Magnum/Magnum.h"
#include "Magnum/visibility.h"

namespace Magnum {

/**
@brief Mesh primitive type

In case of OpenGL, corresponds to @ref GL::MeshPrimitive and is convertible to
it using @ref GL::meshPrimitive(). See documentation of each value for more
information about the mapping.

In case of Vulkan, corresponds to @type_vk_keyword{PrimitiveTopology} and is
convertible to it using @ref Vk::vkPrimitiveTopology(). See documentation of
each value for more information about the mapping. Note that not every mode is available there, use @ref Vk::hasVkPrimitiveTopology() to check for its
presence.
*/
enum class MeshPrimitive: UnsignedInt {
    /**
     * Single points.
     *
     * Corresponds to @ref GL::MeshPrimitive::Points /
     * @def_vk_keyword{PRIMITIVE_TOPOLOGY_POINT_LIST,PrimitiveTopology}.
     */
    Points,

    /**
     * Each pair of vertices defines a single line, lines aren't
     * connected together.
     *
     * Corresponds to @ref GL::MeshPrimitive::Lines /
     * @def_vk_keyword{PRIMITIVE_TOPOLOGY_LINE_LIST,PrimitiveTopology}.
     */
    Lines,

    /**
     * Line strip, last and first vertex are connected together.
     *
     * Corresponds to @ref GL::MeshPrimitive::LineLoop. Not supported on
     * Vulkan.
     */
    LineLoop,

    /**
     * First two vertices define first line segment, each following
     * vertex defines another segment.
     *
     * Corresponds to @ref GL::MeshPrimitive::LineStrip /
     * @def_vk_keyword{PRIMITIVE_TOPOLOGY_LINE_STRIP,PrimitiveTopology}.
     */
    LineStrip,

    /**
     * Each three vertices define one triangle.
     *
     * Corresponds to @ref GL::MeshPrimitive::Triangles /
     * @def_vk_keyword{PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,PrimitiveTopology}.
     */
    Triangles,

    /**
     * First three vertices define first triangle, each following
     * vertex defines another triangle.
     *
     * Corresponds to @ref GL::MeshPrimitive::TriangleStrip /
     * @def_vk_keyword{PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,PrimitiveTopology}.
     */
    TriangleStrip,

    /**
     * First vertex is center, each following vertex is connected to
     * previous and center vertex.
     *
     * Corresponds to @ref GL::MeshPrimitive::TriangleFan /
     * @def_vk_keyword{PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,PrimitiveTopology}.
     */
    TriangleFan
};

/** @debugoperatorenum{MeshPrimitive} */
MAGNUM_EXPORT Debug& operator<<(Debug& debug, MeshPrimitive value);

/**
@brief Mesh index type

In case of OpenGL, corresponds to @ref GL::MeshIndexType and is convertible to
it using @ref GL::meshIndexType(). See documentation of each value for more
information about the mapping.

In case of Vulkan, corresponds to @type_vk_keyword{IndexType} and is
convertible to it using @ref Vk::vkIndexType(). See documentation of each value
for more information about the mapping. Note that not every type is available
there, use @ref Vk::hasVkIndexType() to check for its presence.
@see @ref meshIndexTypeSize()
*/
enum class MeshIndexType: UnsignedInt {
    /**
     * Unsigned byte
     *
     * Corresponds to @ref GL::MeshIndexType::UnsignedByte. Not available on
     * Vulkan. While available on OpenGL, at least AMD GPUs are known to
     * suggest (via debug output) using 16-byte types instead for better
     * efficiency.
     */
    UnsignedByte,

    /**
     * Unsigned short
     *
     * Corresponds to @ref GL::MeshIndexType::UnsignedShort /
     * @def_vk_keyword{INDEX_TYPE_UINT16,IndexType}.
     */
    UnsignedShort,

    /**
     * Unsigned int
     *
     * Corresponds to @ref GL::MeshIndexType::UnsignedInt /
     * @def_vk_keyword{INDEX_TYPE_UINT32,IndexType}.
     */
    UnsignedInt
};

/** @brief Size of given mesh index type */
MAGNUM_EXPORT UnsignedInt meshIndexTypeSize(MeshIndexType type);

/** @debugoperatorenum{MeshIndexType} */
MAGNUM_EXPORT Debug& operator<<(Debug& debug, MeshIndexType value);

}

namespace Corrade { namespace Utility {

/** @configurationvalue{Magnum::MeshPrimitive} */
template<> struct MAGNUM_EXPORT ConfigurationValue<Magnum::MeshPrimitive> {
    ConfigurationValue() = delete;

    /**
     * @brief Writes enum value as string
     *
     * If the value is invalid, returns empty string.
     */
    static std::string toString(Magnum::MeshPrimitive value, ConfigurationValueFlags);

    /**
     * @brief Reads enum value as string
     *
     * If the value is invalid, returns @ref Magnum::MeshPrimitive::Points "MeshPrimitive::Points".
     */
    static Magnum::MeshPrimitive fromString(const std::string& stringValue, ConfigurationValueFlags);
};

/** @configurationvalue{Magnum::MeshIndexType} */
template<> struct MAGNUM_EXPORT ConfigurationValue<Magnum::MeshIndexType> {
    ConfigurationValue() = delete;

    /**
     * @brief Write enum value as string
     *
     * If the value is invalid, returns empty string.
     */
    static std::string toString(Magnum::MeshIndexType value, ConfigurationValueFlags);

    /**
     * @brief Read enum value as string
     *
     * If the value is invalid, returns @ref Magnum::MeshIndexType::UnsignedInt "MeshIndexType::UnsignedInt".
     */
    static Magnum::MeshIndexType fromString(const std::string& stringValue, ConfigurationValueFlags);
};

}}

#endif
