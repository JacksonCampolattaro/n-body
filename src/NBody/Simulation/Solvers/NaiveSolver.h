//
// Created by Jackson Campolattaro on 19/04/2022.
//

#ifndef N_BODY_NAIVESOLVER_H
#define N_BODY_NAIVESOLVER_H

#include "../Solver.h"

namespace NBody {

    class NaiveSolver : public Solver {
    public:

        NaiveSolver(Simulation &simulation) : Solver(simulation) {}

        void step() override;
    };

}


#endif //N_BODY_NAIVESOLVER_H
