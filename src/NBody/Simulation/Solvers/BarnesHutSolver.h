//
// Created by Jackson Campolattaro on 7/2/22.
//

#ifndef N_BODY_BARNESHUTSOLVER_H
#define N_BODY_BARNESHUTSOLVER_H

#include "../Solver.h"

#include "Octree.h"

#include <span>
#include <memory>

namespace NBody {

    class BarnesHutSolver : public Solver {
    private:

        std::unique_ptr<Octree> _octree;

    public:

        BarnesHutSolver(Simulation &simulation, Physics::Rule &rule) : Solver(simulation, rule) {}

        void step() override;

    protected:

        Physics::Acceleration applyRule(Physics::Rule &rule,
                                        const Physics::Position &passivePosition,
                                        const Physics::PassiveMass &passiveMass);

    };

}


#endif //N_BODY_BARNESHUTSOLVER_H
