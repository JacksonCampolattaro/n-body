//
// Created by jackcamp on 4/10/19.
//

#ifndef N_BODY_MODEL_H
#define N_BODY_MODEL_H

#include "PhysicsContext.h"
#include "calculation/Solver.h"
#include "Body.h"


class Model {

public:

    Model(PhysicsContext *physics, Solver *solver);

    /**
     * Leapfrog approximation starts the first increment with a velocity pre-calculation on a half time-step
     */
    void preCalculate(std::vector<Body *> bodies);

    /**
     * Calculates the next time step of the simulation, by calculating forces on all bodies.
     */
    void increment(std::vector<Body *> bodies);


private:

    /*Contains constants and equations used for physics calculations*/
    PhysicsContext *physics;

    /*Algorithms used to perform physics calculations*/
    Solver *solver;

};


#endif //N_BODY_MODEL_H
