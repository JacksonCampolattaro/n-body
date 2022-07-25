//
// Created by Jackson Campolattaro on 7/25/22.
//

#ifndef N_BODY_INSTANCEDFLATRENDERER_H
#define N_BODY_INSTANCEDFLATRENDERER_H

#include <Magnum/Shaders/FlatGL.h>
#include <Magnum/GL/Mesh.h>

#include "../Renderer.h"

namespace NBody {

    using namespace Magnum;
    using namespace Math::Literals;

    class InstancedFlatRenderer : public Renderer {
    public:

        void draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix, const NBody::Simulation &simulation) override;

    protected:

        struct SphereInstanceData {
            Matrix4 transformationMatrix;
            Color3 color;
        };
    };

}


#endif //N_BODY_INSTANCEDFLATRENDERER_H
