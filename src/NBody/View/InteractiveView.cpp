//
// Created by jackcamp on 8/15/21.
//

#include "InteractiveView.h"

NBody::InteractiveView::InteractiveView(NBody::GtkmmArcBallCamera &camera, NBody::Simulation &simulation) :
        View(camera, simulation) {

    signal_button_press_event().connect(camera.slot_buttonPress);
    signal_button_release_event().connect(camera.slot_buttonRelease);

    add_events(
            Gdk::BUTTON_PRESS_MASK |
            Gdk::BUTTON1_MOTION_MASK |
            Gdk::BUTTON2_MOTION_MASK
    );
    signal_motion_notify_event().connect(camera.slot_mouseMotion);

    add_events(
            Gdk::SMOOTH_SCROLL_MASK
    );
    signal_scroll_event().connect(camera.slot_scroll);
}
