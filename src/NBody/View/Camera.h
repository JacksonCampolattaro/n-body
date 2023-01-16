//
// Created by Jackson Campolattaro on 8/25/22.
//

#ifndef N_BODY_CAMERA_H
#define N_BODY_CAMERA_H

#include "Renderers/MultiRenderer.h"

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/MultiSolver.h>

#include <Magnum/Math/Vector2.h>
#include <Magnum/Gl/Framebuffer.h>
#include <spdlog/spdlog.h>

namespace NBody {

    using namespace Magnum;

    class Camera {
    protected:

        MultiRenderer _renderer;

        Color4 _backgroundColor = {};

        sigc::signal<void()> _signal_changed;

    public:

        virtual void draw(const NBody::Simulation &simulation, GL::Framebuffer &framebuffer) = 0;

        virtual void draw(const NBody::Simulation &simulation, const NBody::Solver &solver,
                          GL::Framebuffer &framebuffer) = 0;

        MultiRenderer &renderer() { return _renderer; }

        const Color4 &getBackgroundColor() const { return _backgroundColor; }

        void setBackgroundColor(const Color4 &color) {
            _backgroundColor = color;
            signal_changed().emit();
        }

        virtual float getZoom() const = 0;

        virtual void setZoom(float zoom) = 0;

        virtual Vector3 getPosition() const = 0;

        virtual Vector3 getDirection() const = 0;

        virtual void setPosition(const Vector3 &position) = 0;

    public:

        sigc::signal<void()> &signal_changed() { return _signal_changed; };

    };

    class ControllableCamera : public Camera {
    public:

        virtual void drawHUD(const Vector2i &windowDimensions) = 0;

        virtual void scroll(double dx, double dy) = 0;

        virtual void leftMousePress(const Vector2 &mousePos) = 0;

        virtual void rightMousePress(const Vector2 &mousePos) = 0;

        virtual void mouseRelease() = 0;

        virtual void mouseMotion(const Vector2 &mousePos) = 0;
    };

}

#endif //N_BODY_CAMERA_H
