#ifndef ArcBallCamera_h
#define ArcBallCamera_h

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/AbstractTranslationRotation3D.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>

#include <Magnum/GL/Mesh.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Math/Color.h>

#include <iostream>

#include <NBody/Simulation/Simulation.h>

#include "ArcBall.h"

namespace NBody {

    using namespace Magnum;
    using namespace Math::Literals;

    class ArcBallCamera : public Examples::ArcBall {
    public:

        ArcBallCamera(
                const Vector3 &cameraPosition, const Vector3 &viewCenter,
                const Vector3 &upDir, Deg fov, const Vector2i &windowSize) :
                ArcBall{cameraPosition, viewCenter, upDir, fov, windowSize} {}

        Matrix4 perspectiveProjection() {
            return Matrix4::perspectiveProjection(
                    fov(), Vector2{_windowSize}.aspectRatio(), 0.1f, 10000.0f);
        }

        void draw(const Simulation &simulation);

        void draw(entt::basic_view<entt::entity, entt::exclude_t<>,
                const NBody::Physics::Position,
                const NBody::Graphics::Color,
                const NBody::Graphics::Sphere> view);


        struct SphereInstanceData {
            Matrix4 transformationMatrix;
            Matrix3x3 normalMatrix;
            Color3 color;
        };

        void _draw(entt::basic_view<entt::entity, entt::exclude_t<>,
                const NBody::Physics::Position,
                const NBody::Graphics::Color,
                const NBody::Graphics::Sphere> view);

        void draw();
    };

}

#endif
