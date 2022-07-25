//
// Created by Jackson Campolattaro on 7/24/22.
//

#ifndef N_BODY_INSTANCEDPHONGRENDERER_H
#define N_BODY_INSTANCEDPHONGRENDERER_H

#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/GL/Mesh.h>

#include "../Renderer.h"

namespace NBody {

    using namespace Magnum;
    using namespace Math::Literals;

    class InstancedPhongRenderer : public Renderer {
    public:

        void draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix, const NBody::Simulation &simulation) override;

    protected:

        struct SphereInstanceData {
            Matrix4 transformationMatrix;
            Matrix3x3 normalMatrix;
            Color3 color;
        };
    };

}


#endif //N_BODY_INSTANCEDPHONGRENDERER_H
