//
// Created by Jackson Campolattaro on 7/25/22.
//

#ifndef N_BODY_FLATRENDERER_H
#define N_BODY_FLATRENDERER_H

#include <Magnum/Shaders/FlatGL.h>
#include <Magnum/GL/Mesh.h>

#include "../Renderer.h"

namespace NBody {

    using namespace Magnum;
    using namespace Math::Literals;

    class FlatRenderer : public Renderer {
    public:

        void draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix, const NBody::Simulation &simulation) override;
    };

}


#endif //N_BODY_FLATRENDERER_H
