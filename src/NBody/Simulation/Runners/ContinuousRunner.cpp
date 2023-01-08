//
// Created by Jackson Campolattaro on 12/30/22.
//

#include "ContinuousRunner.h"

void NBody::ContinuousRunner::start() {

    spdlog::debug("Running continuously");

    _idler = Glib::signal_timeout().connect([this] {

        // todo: this shouldn't be a slot
        _solver.slot_step()();

        return true;
    }, 1);
}

void NBody::ContinuousRunner::stop() {
    _idler.disconnect();
}
