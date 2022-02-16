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
    public:
        // Slots

        sigc::slot<bool(double, double)> slot_scroll;

//        sigc::slot<bool(GdkButtonEvent *)> slot_buttonPress;
//        sigc::slot<bool(GdkButtonEvent *)> slot_buttonRelease;
//        sigc::slot<bool(GdkMotionEvent *)> slot_mouseMotion;


    public:

        GtkmmArcBallCamera(
                const Vector3 &cameraPosition, const Vector3 &viewCenter,
                const Vector3 &upDir, Deg fov, const Vector2i &windowSize) :
                ArcBallCamera{cameraPosition, viewCenter, upDir, fov, windowSize} {

            slot_scroll = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_scroll);

//            slot_buttonPress = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_buttonPress);
//            slot_buttonRelease = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_buttonRelease);
//            slot_mouseMotion = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_mouseMotion);
        }

    private:
//
//        bool on_buttonPress(GdkButtonEvent *event);
//
//        bool on_buttonRelease(GdkButtonEvent *event);
//
//        bool on_mouseMotion(GdkMotionEvent *event);

        bool on_scroll(double dx, double dy);

    };

}

#endif //N_BODY_GTKMMARCBALLCAMERA_H
