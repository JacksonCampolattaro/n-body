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

        // TODO projection needs to be driven by information about the window
        Matrix4 _projection{};

    public:

        void setAspectRatio(float aspectRatio);

        void draw(const Simulation::Simulation &simulation);

//        void draw(const entt::basic_view<
//                const NBody::Physics::Position,
//                const NBody::Graphics::Color,
//                const NBody::Graphics::Sphere> &view);
    };
}


#endif //N_BODY_CAMERA_H
