//
// Created by jackcamp on 8/15/21.
//

#include "InteractiveView.h"

NBody::InteractiveView::InteractiveView(Gtk::GLArea::BaseObjectType *cobject,
                                        const Glib::RefPtr<Gtk::Builder> &builder,
                                        NBody::ControllableCamera &camera,
                                        NBody::Simulation &simulation, MultiSolver &solver) :
        SimpleView(cobject, builder, camera, simulation, solver) {

    auto scrollController = Gtk::EventControllerScroll::create();
    scrollController->set_flags(Gtk::EventControllerScroll::Flags::BOTH_AXES);
    scrollController->signal_scroll().connect([&](auto dx, auto dy) {
        camera.scroll(dx, dy);
        return false;
    }, false);
    add_controller(scrollController);

    auto leftClickController = Gtk::GestureClick::create();
    leftClickController->set_touch_only(false);
    leftClickController->set_button(1);
    leftClickController->signal_pressed().connect([&](auto _, auto x, auto y) {
        camera.leftMousePress(toNDC(x, y));
    });
    leftClickController->signal_released().connect([&](auto _, auto x, auto y) {
        camera.mouseRelease();
    });
    add_controller(leftClickController);

    auto middleClickController = Gtk::GestureClick::create();
    middleClickController->set_touch_only(false);
    middleClickController->set_button(2);
    middleClickController->signal_pressed().connect([&](auto _, auto x, auto y) {
        camera.rightMousePress(toNDC(x, y));
    });
    middleClickController->signal_released().connect([&](auto _, auto x, auto y) {
        camera.mouseRelease();
    });
    add_controller(middleClickController);

    // Temporarily set right clicks to act as middle clicks
    auto rightClickController = Gtk::GestureClick::create();
    rightClickController->set_touch_only(false);
    rightClickController->set_button(3);
    rightClickController->signal_pressed().connect([&](auto _, auto x, auto y) {
        camera.rightMousePress(toNDC(x, y));
    });
    rightClickController->signal_released().connect([&](auto _, auto x, auto y) {
        camera.mouseRelease();
    });
    add_controller(rightClickController);

    auto mouseMotionController = Gtk::EventControllerMotion::create();
    mouseMotionController->signal_motion().connect([&](auto x, auto y) {
        camera.mouseMotion(toNDC(x, y));
    });
    add_controller(mouseMotionController);
}

Magnum::Vector2 NBody::InteractiveView::toNDC(float x, float y) {
    return {x * 2.0f / ((float) get_allocated_width()) - 1.0f,
            1.0f - 2.0f * y / ((float) get_allocated_height())};
}
