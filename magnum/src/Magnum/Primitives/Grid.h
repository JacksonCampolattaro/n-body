#ifndef Magnum_Primitives_Grid_h
#define Magnum_Primitives_Grid_h
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
 * @brief Function @ref Magnum::Primitives::grid3DSolid(), @ref Magnum::Primitives::grid3DWireframe()
 */

#include <Corrade/Containers/EnumSet.h>

#include "Magnum/Magnum.h"
#include "Magnum/Math/Math.h"
#include "Magnum/Primitives/visibility.h"
#include "Magnum/Trade/Trade.h"

namespace Magnum { namespace Primitives {

/**
@brief Grid flag

@see @ref GridFlags, @ref grid3DSolid()
*/
enum class GridFlag: UnsignedByte {
    /** Generate texture coordinates with origin in bottom left corner. */
    GenerateTextureCoords = 1 << 0,

    /**
     * Generate normals inn positive Z direction. Disable if you'd be
     * generating your own normals anyway (for example based on a heightmap).
     */
    GenerateNormals = 1 << 1
};

/**
@brief Grid flags

@see @ref grid3DSolid()
*/
typedef Containers::EnumSet<GridFlag> GridFlags;

CORRADE_ENUMSET_OPERATORS(GridFlags)

/**
@brief Solid 3D grid

2x2 grid in the XY plane with normals in positive Z direction. Indexed
@ref MeshPrimitive::Triangles with optional normals and texture coordinates.

@image html primitives-grid3dsolid.png width=256px

The @p subdivisions parameter describes how many times the plane gets cut in
each direction. Specifying @cpp {0, 0} @ce will make the result an (indexed)
equivalent to @ref planeSolid(); @cpp {5, 3} @ce will make the grid have 6
cells horizontally and 4 vertically. In particular, this is different from the
`subdivisions` parameter in @ref icosphereSolid().
@see @ref grid3DWireframe()
*/
MAGNUM_PRIMITIVES_EXPORT Trade::MeshData3D grid3DSolid(const Vector2i& subdivisions, GridFlags flags = GridFlag::GenerateNormals);

/**
@brief Wireframe 3D grid

2x2 grid in the XY plane. Indexed @ref MeshPrimitive::Lines.

@image html primitives-grid3dwireframe.png width=256px

The @p subdivisions parameter describes how many times the plane gets cut in
each direction. Specifying @cpp {0, 0} @ce will make the result an (indexed)
equivalent to @ref planeWireframe(); @cpp {5, 3} @ce will make the grid have 6
cells horizontally and 4 vertically. In particular, this is different from the
`subdivisions` parameter in @ref icosphereSolid().
@see @ref grid3DSolid()
*/
MAGNUM_PRIMITIVES_EXPORT Trade::MeshData3D grid3DWireframe(const Vector2i& subdivisions);

}}

#endif
