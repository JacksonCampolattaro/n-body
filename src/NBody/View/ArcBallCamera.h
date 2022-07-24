#ifndef ArcBallCamera_h
#define ArcBallCamera_h

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/AbstractTranslationRotation3D.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>

#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Math/Color.h>

#include <sigc++/sigc++.h>

#include <iostream>
#include <memory>

#include <NBody/Simulation/Simulation.h>

#include "ArcBall.h"
#include "Renderer.h"
#include "Renderers/InstancedPhongRenderer.h"
#include "Renderers/PhongRenderer.h"

namespace NBody {

    using namespace Magnum;
    using namespace Math::Literals;

    class ArcBallCamera : public Examples::ArcBall {
    protected:

        Color4 _backgroundColor = {0.12, 0.12, 0.1, 1.0};

        InstancedPhongRenderer _renderer;

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

        const Color4 &backgroundColor() { return _backgroundColor; }

    public:
        // Signals

        sigc::signal<void()> signal_changed;
    };

}

#endif
