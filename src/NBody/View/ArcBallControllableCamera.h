//
// Created by Jackson Campolattaro on 8/25/22.
//

#ifndef N_BODY_ARCBALLCONTROLLABLECAMERA_H
#define N_BODY_ARCBALLCONTROLLABLECAMERA_H

#include "ArcBall.h"

#include "Camera.h"

namespace NBody {

    class ArcBallControllableCamera : public ControllableCamera {
    private:

        Magnum::Examples::ArcBall _arcBall;

        enum class TransformationMode {
            ROTATE,
            TRANSLATE,
            NONE
        };

        TransformationMode _mode = TransformationMode::NONE;

        Vector3 _direction; // This is cached, because apparently it's expensive to do in the UI thread

    public:

        ArcBallControllableCamera(const Vector3 &cameraPosition,
                                  const Vector3 &viewCenter,
                                  Deg fov);

        void draw(const NBody::Simulation &simulation, GL::Framebuffer &framebuffer) override;

        void draw(const NBody::Simulation &simulation, const NBody::Solver &solver,
                  GL::Framebuffer &framebuffer) override;

        float getZoom() const override;

        void setZoom(float zoom) override;

        Vector3 getPosition() const override;

        void setPosition(const Vector3 &position) override;

        Vector3 getDirection() const override;

        void drawHUD(const Vector2i &windowDimensions) override;

        void scroll(double dx, double dy) override;

        void leftMousePress(const Vector2 &mousePos) override;

        void rightMousePress(const Vector2 &mousePos) override;

        void mouseRelease() override;

        void mouseMotion(const Vector2 &mousePos) override;
    };

}

#endif //N_BODY_ARCBALLCONTROLLABLECAMERA_H
