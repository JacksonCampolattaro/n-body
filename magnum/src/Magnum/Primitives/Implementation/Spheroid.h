#ifndef Magnum_Primitives_Implementation_Spheroid_h
#define Magnum_Primitives_Implementation_Spheroid_h
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

#include <vector>

#include "Magnum/Magnum.h"
#include "Magnum/Trade/Trade.h"

namespace Magnum { namespace Primitives { namespace Implementation {

class Spheroid {
    public:
        enum class TextureCoords: UnsignedByte {
            DontGenerate,
            Generate
        };

        Spheroid(UnsignedInt segments, TextureCoords textureCoords);

        void capVertex(Float y, Float normalY, Float textureCoordsV);
        void hemisphereVertexRings(UnsignedInt count, Float centerY, Rad startRingAngle, Rad ringAngleIncrement, Float startTextureCoordsV, Float textureCoordsVIncrement);
        void cylinderVertexRings(UnsignedInt count, Float startY, const Vector2& increment, Float startTextureCoordsV, Float textureCoordsVIncrement);
        void bottomFaceRing();
        void faceRings(UnsignedInt count, UnsignedInt offset = 1);
        void topFaceRing();
        void capVertexRing(Float y, Float textureCoordsV, const Vector3& normal);

        Trade::MeshData3D finalize();

    private:
        UnsignedInt segments;
        TextureCoords textureCoords;

        std::vector<UnsignedInt> indices;
        std::vector<Vector3> positions;
        std::vector<Vector3> normals;
        std::vector<Vector2> textureCoords2D;
};

}}}

#endif
