//
// Created by Jackson Campolattaro on 20/04/2022.
//

#ifndef N_BODY_BACKGROUNDSOLVER_H
#define N_BODY_BACKGROUNDSOLVER_H

#include <NBody/Simulation/Solver.h>

#include <glibmm/dispatcher.h>

class BackgroundSolver : public NBody::Solver {
private:

    std::thread *_thread = nullptr;
    Glib::Dispatcher _dispatcher;

    std::shared_ptr<NBody::Solver> _solver;

public:

    template<class SolverType, typename ...Args>
    static BackgroundSolver create(Args &&...args) {
        return BackgroundSolver(std::make_shared<SolverType>(std::forward<Args>(args)...));
    }

    void step() override {
        spdlog::debug("Launching solver in new thread");

        if (_thread != nullptr) {
            spdlog::warn("Attempted to spawn multiple background threads");
            return;
        }

        // Spawn a new thread when the user requests a step
        _thread = new std::thread([&] {

            // Run the underlying solver (this may be very slow)
            _solver->step();

            // Notify the main thread that we're finished (so we can join)
            _dispatcher.emit();
        });
    }

protected:

    template<class SolverType>
    explicit BackgroundSolver(std::shared_ptr<SolverType> solver) :
            _solver(solver), NBody::Solver(solver->simulation()) {

        // When the underlying solver announces that it's complete
        _dispatcher.connect([&] {

            spdlog::debug("Joining completed solver");

            // Join the solver thread
            assert(_thread != nullptr);
            _thread->join();
            _thread = nullptr;

            // Announce the completion to the rest of the UI
            signal_finished.emit();
        });
    }

};


#endif //N_BODY_BACKGROUNDSOLVER_H
