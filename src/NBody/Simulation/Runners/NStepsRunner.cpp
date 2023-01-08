//
// Created by Jackson Campolattaro on 12/30/22.
//

#include "NStepsRunner.h"

void NBody::NStepsRunner::start(std::size_t n) {
    spdlog::debug("Running {} steps", n);

    _nSteps = n;
    _completedSteps = 0;

    _idler = Glib::signal_timeout().connect([this] {

        if (_completedSteps < _nSteps)
            _solver.slot_step()();
        else
            stop();

        return true;
    }, 1);
}

void NBody::NStepsRunner::stop() {
    _idler.disconnect();
    _completedSteps = 0;
    signal_done.emit();
}
