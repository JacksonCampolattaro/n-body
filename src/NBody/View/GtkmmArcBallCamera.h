//
// Created by jackcamp on 8/17/21.
//

#ifndef N_BODY_GTKMMARCBALLCAMERA_H
#define N_BODY_GTKMMARCBALLCAMERA_H


#include <gdk/gdk.h>
#include <gtkmm.h>
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

    public:
        // Slots

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

            slot_scroll = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_scroll);
            slot_leftButtonPress = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_leftButtonPress);
            slot_middleButtonPress = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_middleButtonPress);
            slot_buttonRelease = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_buttonRelease);
            slot_mouseMotion = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_MouseMotion);
        }

    private:

        bool on_scroll(double dx, double dy);

        void on_leftButtonPress(int _, double x, double y);

        void on_middleButtonPress(int _, double x, double y);

        void on_buttonRelease(int _, double x, double y);

        void on_MouseMotion(double x, double y);

    };

}

#endif //N_BODY_GTKMMARCBALLCAMERA_H
