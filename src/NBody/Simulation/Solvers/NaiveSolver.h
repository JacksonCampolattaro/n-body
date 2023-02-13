//
// Created by Jackson Campolattaro on 19/04/2022.
//

#ifndef N_BODY_NAIVESOLVER_H
#define N_BODY_NAIVESOLVER_H

#include <NBody/Simulation/Solver.h>

namespace NBody {

    class NaiveSolver : public Solver {
    public:

        using Solver::Solver;

        std::string id() override { return "naive"; };
        std::string name() override { return "Naive"; };

        void updateAccelerations() override;
    };

}


#endif //N_BODY_NAIVESOLVER_H
