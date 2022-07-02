//
// Created by Jackson Campolattaro on 19/04/2022.
//

#ifndef N_BODY_SOLVER_H
#define N_BODY_SOLVER_H

#include "Simulation.h"

#include <NBody/Physics/Rule.h>

#include <sigc++/sigc++.h>
#include <spdlog/spdlog.h>

#include <thread>

namespace NBody {

    class Solver {
    protected:

        Simulation &_simulation;
        Physics::Rule &_rule;
        float _dt = 0.001;

        sigc::signal<void()> _signal_finished;
        sigc::slot<void()> _slot_step;

    public:

        Solver(Simulation &simulation, Physics::Rule &rule) : _simulation(simulation), _rule(rule) {
            _slot_step = sigc::mem_fun(*this, &Solver::on_step);

            // When the "finished" signal is triggered, all individual particle signals should also trigger
            _signal_finished.connect([&] {
                _simulation.view<sigc::signal<void()>>().each([](auto &s) { s.emit(); });
                _simulation.signal_changed.emit();
            });
        };

        virtual void step() = 0;

        Simulation &simulation() { return _simulation; }

        Physics::Rule &rule() { return _rule; }

    public:

        sigc::signal<void()> &signal_finished() { return _signal_finished; };

        sigc::slot<void()> &slot_step() { return _slot_step; };

    private:

        void on_step() {
            std::thread t([&] {
                std::scoped_lock lock(_simulation.mutex);
                spdlog::debug("Beginning simulation step");
                step();
                spdlog::debug("Finished simulation step");
            });
            t.join();
            signal_finished().emit();
        }

    };

}

#endif //N_BODY_SOLVER_H
