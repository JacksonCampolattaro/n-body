//
// Created by Jackson Campolattaro on 12/29/22.
//

#ifndef N_BODY_RUNNER_H
#define N_BODY_RUNNER_H

#include "Solver.h"

namespace NBody {

    class Runner : public Glib::Object {
    private:

        bool _stop = false;
        std::size_t _remainingSteps = 0;
        std::shared_ptr<std::thread> _thread{};

        TypedDispatcher<std::chrono::duration<double>> _computeTimeDispatcher;

        sigc::slot<void()> _slot_runOneStep;
        sigc::slot<void(std::size_t)> _slot_runNSteps;
        sigc::slot<void()> _slot_runContinuously;
        sigc::slot<void()> _slot_stop;

    protected:
        Solver &_solver;
    public:

        Runner(Solver &solver) :
                Glib::ObjectBase(typeid(NBody::Runner)),
                Glib::Object(),
                _solver(solver) {
            _slot_runOneStep = sigc::mem_fun(*this, &Runner::on_runOneStep);
            _slot_runNSteps = sigc::mem_fun(*this, &Runner::on_runNSteps);
            _slot_runContinuously = sigc::mem_fun(*this, &Runner::on_runContinuously);
            _slot_stop = [&]() {
                _stop = true;
                if (_thread->joinable()) _thread->join();
            };
        }

        virtual std::string id() = 0;

        virtual std::string name() = 0;

        Solver &solver() { return _solver; }

        sigc::slot<void()> &slot_runOneStep() { return _slot_runOneStep; }

        sigc::slot<void(std::size_t)> &slot_runNSteps() { return _slot_runNSteps; };

        sigc::slot<void()> &slot_runContinuously() { return _slot_runContinuously; }

    private:

        void on_runOneStep() {
            spdlog::debug("Running one step");

            // This should be idempotent, calling more than once should only spawn one background thread
            if (_thread) {
                spdlog::trace("Attempted to spawn multiple background threads");
                return;
            }

            spdlog::trace("Launching solver in new thread");

            // Spawn a new thread when the user requests a step
            _thread = std::make_shared<std::thread>([&] { step(); });
        }

        void on_runNSteps(std::size_t n) {
            spdlog::debug("Running {} steps", n);

            // This should be idempotent, calling more than once should only spawn one background thread
            if (_thread) {
                spdlog::trace("Attempted to spawn multiple background threads");
                return;
            }

            spdlog::trace("Launching solver in new thread");

            _remainingSteps = n;
            _thread = std::make_shared<std::thread>([&] {
                while (!_stop && _remainingSteps-- > 0) step();
            });
        }

        void on_runContinuously() {
            spdlog::debug("Running continuously");

            // This should be idempotent, calling more than once should only spawn one background thread
            if (_thread) {
                spdlog::trace("Attempted to spawn multiple background threads");
                return;
            }

            spdlog::trace("Launching solver in new thread");

            _thread = std::make_shared<std::thread>([&] {
                while (!_stop) step();
            });
        }

        void step() {
            spdlog::trace("Beginning simulation step");

            // Cap the number of threads based on the user's preference
            tbb::global_control c{tbb::global_control::max_allowed_parallelism, _solver.maxThreadCount()};

            auto startTime = std::chrono::steady_clock::now();
            _solver.step();
            auto finishTime = std::chrono::steady_clock::now();

            std::chrono::duration<double> duration = finishTime - startTime;
            _computeTimeDispatcher.emit(duration);
            spdlog::trace("Finished simulation step in {} s", duration.count());
        }

    };

}

#endif //N_BODY_RUNNER_H
