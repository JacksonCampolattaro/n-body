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

    class InstancedPhongRenderer : public SimulationRenderer {
    private:

        std::optional<Shaders::PhongGL> _shader{NoCreate};

        std::optional<GL::Mesh> _mesh;

    public:

        InstancedPhongRenderer(const Simulation &simulation) : SimulationRenderer(simulation) {}

        void draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix) override;

    protected:

        struct SphereInstanceData {
            Matrix4 transformationMatrix;
            Matrix3x3 normalMatrix;
            Color3 color;
        };
    };

}


#endif //N_BODY_INSTANCEDPHONGRENDERER_H
