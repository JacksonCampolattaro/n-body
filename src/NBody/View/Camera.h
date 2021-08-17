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
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Math/Quaternion.h>

using namespace Magnum;
using namespace Math::Literals;

namespace NBody {

    class Camera {
    public:

        Matrix4 _projection;
        Matrix4 _transformation;

    private:

        void draw(const Simulation &simulation);

        void draw(entt::basic_view<entt::entity, entt::exclude_t<>,
                const NBody::Physics::Position,
                const NBody::Graphics::Color,
                const NBody::Graphics::Sphere> view);

        void draw();
    };
}


#endif //N_BODY_CAMERA_H
