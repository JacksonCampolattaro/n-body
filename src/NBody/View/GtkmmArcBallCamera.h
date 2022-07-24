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

        sigc::slot<bool(double, double)> slot_scroll;
        sigc::slot<void(int, double, double)> slot_leftButtonPress;
        sigc::slot<void(int, double, double)> slot_middleButtonPress;
        sigc::slot<void(int, double, double)> slot_buttonRelease;
        sigc::slot<void(double, double)> slot_mouseMotion;

    public:

        GtkmmArcBallCamera(
                const Vector3 &cameraPosition, const Vector3 &viewCenter,
                const Vector3 &upDir, Deg fov, const Vector2i &windowSize) :
                ArcBallCamera{cameraPosition, viewCenter, upDir, fov, windowSize} {

            slot_moveTo = sigc::mem_fun(*this, &GtkmmArcBallCamera::setPosition);

            slot_scroll = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_scroll);
            slot_leftButtonPress = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_leftButtonPress);
            slot_middleButtonPress = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_middleButtonPress);
            slot_buttonRelease = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_buttonRelease);
            slot_mouseMotion = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_MouseMotion);
        }

        void setPosition(float x, float y, float z);

        void setZoom(float z);

        void setBackgroundColor(const Gdk::RGBA &color);

    private:

        bool on_scroll(double dx, double dy);

        void on_leftButtonPress(int _, double x, double y);

        void on_middleButtonPress(int _, double x, double y);

        void on_buttonRelease(int _, double x, double y);

        void on_MouseMotion(double x, double y);
    };

}

#endif //N_BODY_GTKMMARCBALLCAMERA_H
