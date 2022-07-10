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

    auto leftClickController = Gtk::GestureClick::create();
    leftClickController->set_touch_only(false);
    leftClickController->set_button(1);
    leftClickController->set_propagation_phase(Gtk::PropagationPhase::BUBBLE);
    leftClickController->signal_pressed().connect(camera.slot_leftButtonPress);
    leftClickController->signal_released().connect(camera.slot_buttonRelease);
    add_controller(leftClickController);

    auto middleClickController = Gtk::GestureClick::create();
    middleClickController->set_touch_only(false);
    middleClickController->set_button(2);
    middleClickController->set_propagation_phase(Gtk::PropagationPhase::BUBBLE);
    middleClickController->signal_pressed().connect(camera.slot_middleButtonPress);
    middleClickController->signal_released().connect(camera.slot_buttonRelease);
    add_controller(middleClickController);

    // Temporarily set right clicks to act as middle clicks
    auto rightClickController = Gtk::GestureClick::create();
    rightClickController->set_touch_only(false);
    rightClickController->set_button(3);
    rightClickController->set_propagation_phase(Gtk::PropagationPhase::BUBBLE);
    rightClickController->signal_pressed().connect(camera.slot_middleButtonPress);
    rightClickController->signal_released().connect(camera.slot_buttonRelease);
    add_controller(rightClickController);

    auto mouseMotionController = Gtk::EventControllerMotion::create();
    mouseMotionController->set_propagation_phase(Gtk::PropagationPhase::BUBBLE);
    mouseMotionController->signal_motion().connect(camera.slot_mouseMotion);
    add_controller(mouseMotionController);
}
