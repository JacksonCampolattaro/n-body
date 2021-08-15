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

namespace NBody {

    class Camera {
    private:

        Matrix4 _projection;
        Matrix4 _transformation{Matrix4::translation(Vector3::zAxis(-100.0f))};

    public:

        void setAspectRatio(float aspectRatio);

        void draw(const Simulation &simulation);

        void move(Matrix4 matrix);

//        void draw(const entt::basic_view<
//                const NBody::Physics::Position,
//                const NBody::Graphics::Color,
//                const NBody::Graphics::Sphere> &view);
    };
}


#endif //N_BODY_CAMERA_H
