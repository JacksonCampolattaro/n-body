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

#include "Enums.h"

#include <Corrade/Containers/ArrayView.h>

#include "Magnum/Mesh.h"
#include "Magnum/PixelFormat.h"
#include "Magnum/Sampler.h"

namespace Magnum { namespace Vk {

namespace {

constexpr VkPrimitiveTopology PrimitiveTopologyMapping[]{
    VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
    VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
    VkPrimitiveTopology(~UnsignedInt{}),
    VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN
};

constexpr VkIndexType IndexTypeMapping[]{
    VkIndexType(~UnsignedInt{}),
    VK_INDEX_TYPE_UINT16,
    VK_INDEX_TYPE_UINT32
};

#ifndef DOXYGEN_GENERATING_OUTPUT /* It gets *really* confused */
static_assert(VK_FORMAT_UNDEFINED == 0, "VK_FORMAT_UNDEFINED is assumed to be 0");

constexpr VkFormat FormatMapping[] {
    #define _c(input, format) VK_FORMAT_ ## format,
    #define _s(input) {},
    #include "Magnum/Vk/Implementation/formatMapping.hpp"
    #undef _s
    #undef _c
};

constexpr VkFormat CompressedFormatMapping[] {
    #define _c(input, format) VK_FORMAT_ ## format,
    #define _s(input) {},
    #include "Magnum/Vk/Implementation/compressedFormatMapping.hpp"
    #undef _s
    #undef _c
};
#endif

constexpr VkFilter FilterMapping[]{
    VK_FILTER_NEAREST,
    VK_FILTER_LINEAR
};

constexpr VkSamplerMipmapMode SamplerMipmapModeMapping[]{
    VK_SAMPLER_MIPMAP_MODE_NEAREST, /* See vkSamplerMipmapMode() for details */
    VK_SAMPLER_MIPMAP_MODE_NEAREST,
    VK_SAMPLER_MIPMAP_MODE_LINEAR
};

constexpr VkSamplerAddressMode SamplerAddressModeMapping[]{
    VK_SAMPLER_ADDRESS_MODE_REPEAT,
    VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
    VkSamplerAddressMode(~UnsignedInt{}),
};

}

bool hasVkPrimitiveTopology(const Magnum::MeshPrimitive primitive) {
    CORRADE_ASSERT(UnsignedInt(primitive) < Containers::arraySize(PrimitiveTopologyMapping),
        "Vk::hasVkPrimitiveTopology(): invalid primitive" << primitive, {});
    return UnsignedInt(PrimitiveTopologyMapping[UnsignedInt(primitive)]) != ~UnsignedInt{};
}

VkPrimitiveTopology vkPrimitiveTopology(const Magnum::MeshPrimitive primitive) {
    CORRADE_ASSERT(UnsignedInt(primitive) < Containers::arraySize(PrimitiveTopologyMapping),
        "Vk::vkPrimitiveTopology(): invalid primitive" << primitive, {});
    const VkPrimitiveTopology out = PrimitiveTopologyMapping[UnsignedInt(primitive)];
    CORRADE_ASSERT(out != VkPrimitiveTopology(~UnsignedInt{}),
        "Vk::vkPrimitiveTopology(): unsupported primitive" << primitive, {});
    return out;
}

bool hasVkIndexType(const Magnum::MeshIndexType type) {
    CORRADE_ASSERT(UnsignedInt(type) < Containers::arraySize(IndexTypeMapping),
        "Vk::hasVkIndexType(): invalid type" << type, {});
    return UnsignedInt(IndexTypeMapping[UnsignedInt(type)]) != ~UnsignedInt{};
}

VkIndexType vkIndexType(const Magnum::MeshIndexType type) {
    CORRADE_ASSERT(UnsignedInt(type) < Containers::arraySize(IndexTypeMapping),
        "Vk::vkIndexType(): invalid type" << type, {});
    const VkIndexType out = IndexTypeMapping[UnsignedInt(type)];
    CORRADE_ASSERT(out != VkIndexType(~UnsignedInt{}),
        "Vk::vkIndexType(): unsupported type" << type, {});
    return out;
}

bool hasVkFormat(const Magnum::PixelFormat format) {
    if(isPixelFormatImplementationSpecific(format))
        return true;

    CORRADE_ASSERT(UnsignedInt(format) - 1 < Containers::arraySize(FormatMapping),
        "Vk::hasVkFormat(): invalid format" << format, {});
    return UnsignedInt(FormatMapping[UnsignedInt(format) - 1]);
}

bool hasVkFormat(const Magnum::CompressedPixelFormat format) {
    if(isCompressedPixelFormatImplementationSpecific(format))
        return true;

    CORRADE_ASSERT(UnsignedInt(format) - 1 < Containers::arraySize(CompressedFormatMapping),
        "Vk::hasVkFormat(): invalid format" << format, {});
    return UnsignedInt(CompressedFormatMapping[UnsignedInt(format) - 1]);
}

VkFormat vkFormat(const Magnum::PixelFormat format) {
    if(isPixelFormatImplementationSpecific(format))
        return pixelFormatUnwrap<VkFormat>(format);

    CORRADE_ASSERT(UnsignedInt(format) - 1 < Containers::arraySize(FormatMapping),
        "Vk::vkFormat(): invalid format" << format, {});
    const VkFormat out = FormatMapping[UnsignedInt(format) - 1];
    CORRADE_ASSERT(UnsignedInt(out),
        "Vk::vkFormat(): unsupported format" << format, {});
    return out;
}

VkFormat vkFormat(const Magnum::CompressedPixelFormat format) {
    if(isCompressedPixelFormatImplementationSpecific(format))
        return compressedPixelFormatUnwrap<VkFormat>(format);

    CORRADE_ASSERT(UnsignedInt(format) - 1 < Containers::arraySize(CompressedFormatMapping),
        "Vk::vkFormat(): invalid format" << format, {});
    const VkFormat out = CompressedFormatMapping[UnsignedInt(format) - 1];
    CORRADE_ASSERT(UnsignedInt(out),
        "Vk::vkFormat(): unsupported format" << format, {});
    return out;
}

VkFilter vkFilter(const Magnum::SamplerFilter filter) {
    CORRADE_ASSERT(UnsignedInt(filter) < Containers::arraySize(FilterMapping),
        "Vk::vkFilter(): invalid filter" << filter, {});
    return FilterMapping[UnsignedInt(filter)];
}

VkSamplerMipmapMode vkSamplerMipmapMode(const Magnum::SamplerMipmap mipmap) {
    CORRADE_ASSERT(UnsignedInt(mipmap) < Containers::arraySize(SamplerMipmapModeMapping),
        "Vk::vkSamplerMipmapMode(): invalid mode" << mipmap, {});
    return SamplerMipmapModeMapping[UnsignedInt(mipmap)];
}

bool hasVkSamplerAddressMode(const Magnum::SamplerWrapping wrapping) {
    CORRADE_ASSERT(UnsignedInt(wrapping) < Containers::arraySize(SamplerAddressModeMapping),
        "Vk::hasVkSamplerAddressMode(): invalid wrapping" << wrapping, {});
    return UnsignedInt(SamplerAddressModeMapping[UnsignedInt(wrapping)]) != ~UnsignedInt{};
}

VkSamplerAddressMode vkSamplerAddressMode(const Magnum::SamplerWrapping wrapping) {
    CORRADE_ASSERT(UnsignedInt(wrapping) < Containers::arraySize(SamplerAddressModeMapping),
        "Vk::vkSamplerAddressMode(): invalid wrapping" << wrapping, {});
    const VkSamplerAddressMode out = SamplerAddressModeMapping[UnsignedInt(wrapping)];
    CORRADE_ASSERT(out != VkSamplerAddressMode(~UnsignedInt{}),
        "Vk::vkSamplerAddressMode(): unsupported wrapping" << wrapping, {});
    return out;
}

}}
