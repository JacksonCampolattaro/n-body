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
        float _theta = 0.5f;

    public:

        BarnesHutSolver(Simulation &simulation, Physics::Rule &rule) : Solver(simulation, rule) {}

        void step() override;

    };

}


#endif //N_BODY_BARNESHUTSOLVER_H
