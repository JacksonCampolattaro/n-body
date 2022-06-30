//
// Created by Jackson Campolattaro on 20/04/2022.
//

#ifndef N_BODY_BACKGROUNDSOLVER_H
#define N_BODY_BACKGROUNDSOLVER_H

#include <NBody/Simulation/Solver.h>

#include <glibmm/dispatcher.h>

template<class BaseSolver>
class BackgroundSolver : public BaseSolver {
private:

    std::shared_ptr<std::thread> _thread;
    Glib::Dispatcher _dispatcher;

public:

    template<typename ...Args>
    explicit BackgroundSolver(Args &&...args) : BaseSolver(std::forward<Args>(args)...) {

        // When the underlying solver announces that it's complete...
        _dispatcher.connect([&] {

            spdlog::debug("Joining completed solver");

            // Join the solver thread
            assert(_thread != nullptr);
            _thread->join();
            _thread = nullptr;

            // Announce the completion to the rest of the UI
            BaseSolver::signal_finished().emit();
        });
    }

    ~BackgroundSolver() {
        if (_thread)
            _thread->join();
    }

    void step() override {

        // This should be idempotent, calling more than once should only spawn one background thread
        if (_thread != nullptr) {
            spdlog::trace("Attempted to spawn multiple background threads");
            return;
        }

        spdlog::debug("Launching solver in new thread");

        // Spawn a new thread when the user requests a step
        _thread = std::make_shared<std::thread>([&] {

            // Run the underlying solver (this may be very slow)
            BaseSolver::step();

            // Notify the main thread that we're finished (so we can join)
            _dispatcher.emit();
        });
    }

};


#endif //N_BODY_BACKGROUNDSOLVER_H
