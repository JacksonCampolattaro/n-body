//
// Created by jackcamp on 4/10/19.
//

#ifndef N_BODY_NAIVESOLVER_H
#define N_BODY_NAIVESOLVER_H


#include "../Solver.h"


class NaiveSolver : Solver {

public:

    void solve(std::vector<Body *> bodies, PhysicsContext *phys) override;

};


#endif //N_BODY_NAIVESOLVER_H