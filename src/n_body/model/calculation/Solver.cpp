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

/*Solver::type_signal_preparing_solver Solver::signal_preparing_solver() {
    return m_signal_preparing_solver;
}

Solver::type_signal_solving Solver::signal_solving() {
    return m_signal_solving;
}

Solver::type_signal_shifting_buffers Solver::signal_shifting_buffers() {
    return m_signal_shifting_buffers;
}

Solver::type_signal_complete Solver::signal_complete() {
    return m_signal_complete;
}*/
