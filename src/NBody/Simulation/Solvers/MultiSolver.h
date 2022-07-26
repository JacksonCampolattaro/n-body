//
// Created by Jackson Campolattaro on 7/16/22.
//

#ifndef N_BODY_MULTISOLVER_H
#define N_BODY_MULTISOLVER_H

#include <NBody/Simulation/Solver.h>
#include <NBody/Simulation/Solvers/NaiveSolver.h>

namespace NBody {

    class MultiSolver : public Solver {
    private:

        Solver *_activeSolver;

        sigc::connection _currentStatusConnection;

    public:

        MultiSolver(Simulation &simulation, Physics::Rule &rule) :
                Solver(simulation, rule),
                _activeSolver(&getSolver<NaiveSolver>()) {}

        template<class SolverType>
        SolverType &getSolver() {
            static SolverType solver{_simulation, _rule};
            return solver;
        }

        Solver &activeSolver() {
            return *_activeSolver;
        }

        template<class SolverType>
        void select() {
            _activeSolver = &getSolver<SolverType>();
            _currentStatusConnection = _activeSolver->signal_status().connect(signal_status().make_slot());
        }

        void step() override {
            activeSolver().step();
        }
    };

}

#endif //N_BODY_MULTISOLVER_H
