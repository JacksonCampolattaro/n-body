//
// Created by Jackson Campolattaro on 12/30/22.
//

#include "OneStepRunner.h"

void NBody::OneStepRunner::step() {

    spdlog::debug("Running one step");

    // todo: this shouldn't be a slot
    _solver.slot_step()();
}
