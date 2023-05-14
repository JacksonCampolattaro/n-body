//
// Created by Jackson Campolattaro on 1/7/23.
//

#include "OneStepRunnerController.h"

UI::OneStepRunnerController::OneStepRunnerController(NBody::OneStepRunner<Gravity> &runner) :
        BuilderWidget<Gtk::Box>("/ui/one_step_runner_controller.xml"),
        _runner(runner),
        _stepButton(getWidget<Gtk::ToggleButton>("step-button")) {

    _stepButton.signal_clicked().connect([&]() {
        _runner.step();
        _stepButton.set_active(false);
    });

    _runner.solver().signal_finished().connect([&](){
        _stepButton.set_active(true);
    });
}
