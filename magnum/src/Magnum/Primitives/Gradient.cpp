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

#include "Gradient.h"

#include "Magnum/Mesh.h"
#include "Magnum/Math/Color.h"
#include "Magnum/Math/Intersection.h"
#include "Magnum/Trade/MeshData2D.h"
#include "Magnum/Trade/MeshData3D.h"

namespace Magnum { namespace Primitives {

Trade::MeshData2D gradient2D(const Vector2& a, const Color4& colorA, const Vector2& b, const Color4& colorB) {
    std::vector<Vector2> positions{Vector2{ 1.0f, -1.0f},
                                   Vector2{ 1.0f,  1.0f},
                                   Vector2{-1.0f, -1.0f},
                                   Vector2{-1.0f,  1.0f}};
    std::vector<Color4> colors{4};

    /* For every corner, take a line perpendicular to the gradient direction
       and passing through the corner. The calculated intersection position
       with the gradient line segment is the blend factor for the two colors
       for given corner. */
    const Vector2 direction = b - a;
    const Vector2 perpendicular = direction.perpendicular();
    for(std::size_t i = 0; i != 4; ++i) {
        const Float t = Math::Intersection::lineSegmentLine(a, direction, positions[i], perpendicular);
        colors[i] = Math::lerp(colorA, colorB, t);
    }

    return Trade::MeshData2D{MeshPrimitive::TriangleStrip, {}, {std::move(positions)}, {}, {std::move(colors)}, nullptr};
}

Trade::MeshData2D gradient2DHorizontal(const Color4& colorLeft, const Color4& colorRight) {
    return Primitives::gradient2D({-1.0f, 0.0f}, colorLeft, {1.0f, 0.0f}, colorRight);
}

Trade::MeshData2D gradient2DVertical(const Color4& colorBottom, const Color4& colorTop) {
    return Primitives::gradient2D({0.0f, -1.0f}, colorBottom, {0.0f, 1.0f}, colorTop);
}

Trade::MeshData3D gradient3D(const Vector3& a, const Color4& colorA, const Vector3& b, const Color4& colorB) {
    std::vector<Vector3> positions{Vector3{ 1.0f, -1.0f, 0.0f},
                                   Vector3{ 1.0f,  1.0f, 0.0f},
                                   Vector3{-1.0f, -1.0f, 0.0f},
                                   Vector3{-1.0f,  1.0f, 0.0f}};
    std::vector<Color4> colors{4};

    /* For every corner, take a plane perpendicular to the gradient direction
       and passing through the corner. The calculated intersection position
       with the gradient line segment is the blend factor for the two colors
       for given corner. */
    const Vector3 direction = b - a;
    for(std::size_t i = 0; i != 4; ++i) {
        const Vector4 plane = Math::planeEquation(direction, positions[i]);
        const Float t = Math::Intersection::planeLine(plane, a, direction);
        colors[i] = Math::lerp(colorA, colorB, t);
    }

    return Trade::MeshData3D{MeshPrimitive::TriangleStrip, {}, {std::move(positions)}, {{
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f}
    }}, {}, {std::move(colors)}, nullptr};
}

Trade::MeshData3D gradient3DHorizontal(const Color4& colorLeft, const Color4& colorRight) {
    return Primitives::gradient3D({-1.0f, 0.0f, 0.0f}, colorLeft, {1.0f, 0.0f, 0.0f}, colorRight);
}

Trade::MeshData3D gradient3DVertical(const Color4& colorBottom, const Color4& colorTop) {
    return Primitives::gradient3D({0.0f, -1.0f, 0.0f}, colorBottom, {0.0f, 1.0f, 0.0f}, colorTop);
}

}}
