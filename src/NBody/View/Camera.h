//
// Created by jackcamp on 7/19/21.
//

#ifndef N_BODY_CAMERA_H
#define N_BODY_CAMERA_H

#include <NBody/Simulation/Simulation.h>

#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Shaders/Flat.h>

#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Color.h>

#include <Magnum/GL/Mesh.h>

using namespace Magnum;
using namespace Math::Literals;

using namespace NBody::Simulation;

namespace NBody {

    class Camera {
    private:

        Shaders::PhongGL _shader{NoCreate};

        // TODO projection needs to be driven by information about the window
        Matrix4 _projection{
                Matrix4::perspectiveProjection(35.0_degf, 1.0, 0.01f, 1000.0f) *
                Matrix4::translation(Vector3::zAxis(-50.0f))
        };

        // FIXME temporary
        GL::Buffer _sphereInstanceBuffer{NoCreate};
        Containers::Array<Graphics::Sphere::InstanceData> _sphereInstanceData;

    public:

        void setAspectRatio(float aspectRatio);

        void draw(const Simulation::Simulation &simulation);

    private:

        void draw(const entt::view<
                const NBody::Physics::Position,
                const NBody::Graphics::Color,
                const NBody::Graphics::Sphere
        > &view);
    };
}


#endif //N_BODY_CAMERA_H
