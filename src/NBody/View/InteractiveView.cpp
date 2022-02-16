//
// Created by jackcamp on 8/15/21.
//

#include "InteractiveView.h"

NBody::InteractiveView::InteractiveView(NBody::GtkmmArcBallCamera &camera, NBody::Simulation &simulation) :
        View(camera, simulation) {

    auto scrollController = Gtk::EventControllerScroll::create();
    scrollController->set_flags(Gtk::EventControllerScroll::Flags::BOTH_AXES);
    scrollController->set_propagation_phase(Gtk::PropagationPhase::BUBBLE);
    scrollController->signal_scroll().connect(camera.slot_scroll, false);
    add_controller(scrollController);


//    signal_button_press_event().connect(camera.slot_buttonPress);
//    signal_button_release_event().connect(camera.slot_buttonRelease);
//
//    add_events(
//            Gdk::BUTTON_PRESS_MASK |
//            Gdk::BUTTON1_MOTION_MASK |
//            Gdk::BUTTON2_MOTION_MASK
//    );
//    signal_motion_notify_event().connect(camera.slot_mouseMotion);
//
//    add_events(
//            Gdk::SMOOTH_SCROLL_MASK
//    );
//    signal_scroll_event().connect(camera.slot_scroll);
}
