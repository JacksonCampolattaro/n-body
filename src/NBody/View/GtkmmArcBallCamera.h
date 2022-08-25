//
// Created by jackcamp on 8/17/21.
//

#ifndef N_BODY_GTKMMARCBALLCAMERA_H
#define N_BODY_GTKMMARCBALLCAMERA_H


#include <gtkmm.h>
#include <glibmm/propertyproxy.h>
#include <sigc++/slot.h>

#include <spdlog/spdlog.h>

#include "ArcBallCamera.h"

using namespace Magnum;

namespace NBody {

    class GtkmmArcBallCamera : public ArcBallCamera {
    private:

        enum class TransformationMode {
            ROTATE,
            TRANSLATE,
            NONE
        };

        TransformationMode _mode = TransformationMode::NONE;

    public: // Signals

        sigc::signal<void(float, float, float)> signal_positionChanged;
        sigc::signal<void(float, float, float)> signal_directionChanged;
        sigc::signal<void(float)> signal_zoomChanged;

    public: // Slots

        sigc::slot<void(float, float, float)> slot_moveTo;

    public:

        GtkmmArcBallCamera(
                const Vector3 &cameraPosition, const Vector3 &viewCenter,
                const Vector3 &upDir, Deg fov) :
                ArcBallCamera{cameraPosition, viewCenter, upDir, fov} {

            slot_moveTo = sigc::mem_fun(*this, &GtkmmArcBallCamera::setPosition);
        }

        void setPosition(float x, float y, float z);

        void setZoom(float z);

        void setBackgroundColor(const Gdk::RGBA &color);

        bool on_scroll(double dx, double dy);

        void on_leftButtonPress(Vector2 mousePosNDC);

        void on_middleButtonPress(Vector2 mousePosNDC);

        void on_buttonRelease();

        void on_mouseMotion(Vector2 mousePosNDC);
    };

}

#endif //N_BODY_GTKMMARCBALLCAMERA_H
