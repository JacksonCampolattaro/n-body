//
// Created by Jackson Campolattaro on 19/04/2022.
//

#ifndef N_BODY_SOLVER_H
#define N_BODY_SOLVER_H

#include "Simulation.h"

#include <sigc++/sigc++.h>
#include <spdlog/spdlog.h>

#include <thread>

namespace NBody {

    class Solver {
    protected:

        Simulation &_simulation;
        float _dt = 1.0;

    public:

        explicit Solver(Simulation &simulation) : _simulation(simulation) {
            slot_step = sigc::mem_fun(*this, &Solver::on_step);
        };

        virtual void step() = 0;

    public:

        sigc::signal<void()> signal_finished;

    public:

        sigc::slot<void()> slot_step;

    private:

        void on_step() {
            std::thread t([&] {
                std::scoped_lock lock(_simulation.mutex);
                spdlog::debug("Beginning simulation step");
                step();
                spdlog::debug("Finished simulation step");
            });
            t.join();
            signal_finished.emit();
        }

    };

}

#endif //N_BODY_SOLVER_H
