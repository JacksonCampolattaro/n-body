//
// Created by Jackson Campolattaro on 1/25/23.
//

#ifndef N_BODY_SPRITERENDERER_H
#define N_BODY_SPRITERENDERER_H

#include <Corrade/Containers/GrowableArray.h>

#include <Magnum/Shaders/FlatGL.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Primitives/Circle.h>

#include "../Renderer.h"

namespace NBody {

    using namespace Magnum;
    using namespace Math::Literals;

    class SpriteRenderer : public SimulationRenderer {
    public:

        SpriteRenderer(const Simulation &simulation) : SimulationRenderer(simulation) {}

        std::string id() override { return "circle"; };

        std::string name() override { return "Circle"; };

        void draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix) override;

    protected:

        struct CircleInstanceData {
            Matrix4 transformationMatrix;
            Color4 color;
        };
    };

}


#endif //N_BODY_SPRITERENDERER_H
