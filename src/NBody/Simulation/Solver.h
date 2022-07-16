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

        std::shared_ptr<std::thread> _thread;
        Glib::Dispatcher _dispatcher;

    public:

        Solver(Simulation &simulation, Physics::Rule &rule) : _simulation(simulation), _rule(rule) {
            _slot_step = sigc::mem_fun(*this, &Solver::on_step);

            // When the underlying solver announces that it's complete...
            _dispatcher.connect([&] {

                spdlog::trace("Joining completed solver");

                // Join the solver thread
                assert(_thread);
                _thread->join();
                _thread.reset();

                // Announce the completion to the rest of the UI
                _simulation.view<sigc::signal<void()>>().each([](auto &s) { s.emit(); });
                _simulation.signal_changed.emit();
                signal_finished().emit();
            });
        };

        virtual ~Solver() {
            if (_thread)
                _thread->join();
            _thread.reset();
        }

        virtual void step() = 0;

        Simulation &simulation() { return _simulation; }

        Physics::Rule &rule() { return _rule; }

    public:

        sigc::signal<void()> &signal_finished() { return _signal_finished; };

        sigc::signal<void(std::chrono::duration<double>)> &
        signal_computeTime() { return _computeTimeDispatcher.signal(); };

        sigc::slot<void()> &slot_step() { return _slot_step; };

    private:

        void on_step() {

            // This should be idempotent, calling more than once should only spawn one background thread
            if (_thread) {
                spdlog::trace("Attempted to spawn multiple background threads");
                return;
            }

            spdlog::trace("Launching solver in new thread");

            // Spawn a new thread when the user requests a step
            _thread = std::make_shared<std::thread>([&] {

                spdlog::trace("Beginning simulation step");

                auto startTime = std::chrono::high_resolution_clock::now();
                step();
                auto finishTime = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double> duration = finishTime - startTime;
                _computeTimeDispatcher.emit(duration);
                spdlog::trace("Finished simulation step in {} s", duration.count());

                // Notify the main thread that we're finished (so we can join)
                _dispatcher.emit();
            });
        }
    };

}

#endif //N_BODY_SOLVER_H
