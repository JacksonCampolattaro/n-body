//
// Created by Jackson Campolattaro on 1/7/23.
//

#include "ContinuousRunnerController.h"

UI::ContinuousRunnerController::ContinuousRunnerController(NBody::ContinuousRunner<Gravity> &runner) :
        BuilderWidget<Gtk::Box>("/ui/continuous_runner_controller.xml"),
        _runner(runner),
        _runButton(getWidget<Gtk::ToggleButton>("run-button")) {

    _runButton.signal_toggled().connect([&]() {
        if (_runButton.get_active()) {
            _runner.start();
            _runButton.set_label("Stop");
        } else {
            _runner.stop();
            _runButton.set_label("Start");
        }
    });
}
