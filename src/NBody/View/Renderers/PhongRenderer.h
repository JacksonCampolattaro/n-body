//
// Created by Jackson Campolattaro on 7/24/22.
//

#ifndef N_BODY_PHONGRENDERER_H
#define N_BODY_PHONGRENDERER_H

#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/GL/Mesh.h>

#include "../Renderer.h"

namespace NBody {

    using namespace Magnum;
    using namespace Math::Literals;

    class PhongRenderer : public SimulationRenderer {
    public:

        PhongRenderer(const Simulation &simulation) : SimulationRenderer(simulation) {}

        void draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix) override;
    };

}


#endif //N_BODY_PHONGRENDERER_H
