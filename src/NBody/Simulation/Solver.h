//
// Created by Jackson Campolattaro on 19/04/2022.
//

#ifndef N_BODY_SOLVER_H
#define N_BODY_SOLVER_H

#include "Simulation.h"
#include "TypedDispatcher.h"

#include <NBody/Physics/Rule.h>

#include <sigc++/sigc++.h>
#include <spdlog/spdlog.h>

namespace NBody {

    class Solver {
    protected:

        Simulation &_simulation;
        Physics::Rule &_rule;
        float _dt = 0.001;

        sigc::signal<void()> _signal_finished;
        sigc::slot<void()> _slot_step;

        TypedDispatcher<std::chrono::duration<double>> _computeTimeDispatcher;

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

        sigc::signal<void(std::chrono::duration<double>)> &signal_computeTime() { return _computeTimeDispatcher.signal(); };

        sigc::slot<void()> &slot_step() { return _slot_step; };

    private:

        void on_step() {
            std::scoped_lock lock(_simulation.mutex);
            spdlog::trace("Beginning simulation step");
            auto startTime = std::chrono::high_resolution_clock::now();
            step();
            auto finishTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = finishTime - startTime;
            spdlog::debug("Finished simulation step in {}s", duration.count());
            signal_finished().emit();
        }

    };

}

#endif //N_BODY_SOLVER_H
