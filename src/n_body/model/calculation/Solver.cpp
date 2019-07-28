//
// Created by jackcamp on 4/5/19.
//

#include "Solver.h"

Solver *Solver::enableThreading(bool enabled) {

    this->threadingEnabled = enabled;

    return this;
}

bool Solver::isThreadingEnabled() const {
    return threadingEnabled;
}

Solver::type_signal_complete Solver::signal_complete() {
    return m_signal_complete;
}
