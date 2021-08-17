//
// Created by jackcamp on 8/17/21.
//

#ifndef N_BODY_GTKMMARCBALLCAMERA_H
#define N_BODY_GTKMMARCBALLCAMERA_H


#include <gdk/gdk.h>
#include <sigc++/slot.h>

#include "ArcBallCamera.h"

using namespace Magnum;

namespace NBody {

    class GtkmmArcBallCamera : public Magnum::Examples::ArcBallCamera {
    public:
        // Slots

        sigc::slot<bool(GdkEventButton *)> slot_buttonPress;
        sigc::slot<bool(GdkEventButton *)> slot_buttonRelease;
        sigc::slot<bool(GdkEventMotion *)> slot_mouseMotion;
        sigc::slot<bool(GdkEventScroll *)> slot_scroll;


    public:

        GtkmmArcBallCamera(
                const Vector3 &cameraPosition, const Vector3 &viewCenter,
                const Vector3 &upDir, Deg fov, const Vector2i &windowSize,
                const Vector2i &viewportSize) :
                Examples::ArcBallCamera{cameraPosition, viewCenter, upDir, fov, windowSize, viewportSize} {

            slot_buttonPress = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_buttonPress);
            slot_buttonRelease = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_buttonRelease);
            slot_mouseMotion = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_mouseMotion);
            slot_scroll = sigc::mem_fun(*this, &GtkmmArcBallCamera::on_scroll);
        }

    private:

        bool on_buttonPress(GdkEventButton *event);

        bool on_buttonRelease(GdkEventButton *event);

        bool on_mouseMotion(GdkEventMotion *event);

        bool on_scroll(GdkEventScroll *event);

    };

}

#endif //N_BODY_GTKMMARCBALLCAMERA_H
