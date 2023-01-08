//
// Created by Jackson Campolattaro on 1/7/23.
//

#include "NStepsRunnerController.h"

static std::string validate(const std::string &in) {
    auto filtered = in;
    std::erase_if(filtered, [](const auto &c) {
        return !std::isdigit(c);
    });
    return filtered.empty() ? "1" : filtered;
}


UI::NStepsRunnerController::NStepsRunnerController(NBody::NStepsRunner &runner) :
        BuilderWidget<Gtk::Box>("/ui/n_steps_runner_controller.xml"),
        _runner(runner),
        _stepCountEntry(getWidget<Gtk::Entry>("step-count-entry")),
        _runButton(getWidget<Gtk::ToggleButton>("run-button")) {

    // todo: use the right label if the runner is already started

    _runButton.signal_toggled().connect([&]() {
        if (_runButton.get_active()) {
            _stepCountEntry.set_text(validate(_stepCountEntry.get_text()));
            _runner.start(std::stoi(_stepCountEntry.get_text()));
            _runButton.set_label("Stop");
        } else {
            _runner.stop();
        }
    });

    _runner.solver().signal_finished().connect([&]() {
        int requestedSteps = std::stoi(_stepCountEntry.get_text());

        float progress = (float) _runner.completedSteps() / (float) requestedSteps;
        _stepCountEntry.set_progress_fraction(progress);
    });

    _stepCountEntry.signal_editing_done().connect([&]() {
        _stepCountEntry.set_text(validate(_stepCountEntry.get_text()));
        _stepCountEntry.set_progress_fraction(0.0f);
    });

    _runner.signal_done.connect([&](){
        _stepCountEntry.set_progress_fraction(0.0f);
        _runButton.set_active(false);
        _runButton.set_label("Start");
    });
}
