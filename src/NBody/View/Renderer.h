//
// Created by Jackson Campolattaro on 7/24/22.
//

#ifndef N_BODY_RENDERER_H
#define N_BODY_RENDERER_H

#include <Magnum/GL/Renderer.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Math/Color.h>

#include <NBody/Simulation/Simulation.h>

namespace NBody {

    class Renderer {
    public:

        virtual void draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix,
                          const NBody::Simulation &simulation) = 0;

    };
}

#endif //N_BODY_RENDERER_H
