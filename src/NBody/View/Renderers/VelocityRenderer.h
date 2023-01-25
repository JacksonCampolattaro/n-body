//
// Created by Jackson Campolattaro on 1/25/23.
//

#ifndef N_BODY_VELOCITYRENDERER_H
#define N_BODY_VELOCITYRENDERER_H

#include <Corrade/Containers/GrowableArray.h>

#include <Magnum/Shaders/FlatGL.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Primitives/Line.h>

#include "../Renderer.h"

namespace NBody {

    using namespace Magnum;
    using namespace Math::Literals;

    class VelocityRenderer : public SimulationRenderer {
    public:

        VelocityRenderer(const Simulation &simulation) : SimulationRenderer(simulation) {}

        void draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix) override;

    protected:

        struct LineInstanceData {
            Matrix4 transformationMatrix;
            Color4 color;
        };
    };

}


#endif //N_BODY_VELOCITYRENDERER_H
