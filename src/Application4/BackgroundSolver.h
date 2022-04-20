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
        _thread = new std::thread([&] {
            _solver->step();
            _dispatcher.emit();
        });
    }

protected:

    template<class SolverType>
    explicit BackgroundSolver(std::shared_ptr<SolverType> solver) :
            _solver(solver), NBody::Solver(solver->simulation()) {

        // When the underlying solver announces that it's complete
        _dispatcher.connect([&] {

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
