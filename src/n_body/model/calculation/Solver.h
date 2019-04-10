//
// Created by jackcamp on 4/5/19.
//

#ifndef N_BODY_SOLVER_H
#define N_BODY_SOLVER_H

#include "../Body.h" // The Solver contains bodies
#include "../PhysicsContext.h" // The solver affects the bodies according to the physics context it's given

#include <vector> // Used to handle the list of bodies in the Simulation

class Solver {

public:

    Solver(PhysicsContext* phys);

    /**
     * Calculates forces between bodies and applies them to each body
     * @param bodies the list of bodies to perform calculations on
     */
    virtual void solve(std::vector<Body *> bodies) = 0;

private:

    PhysicsContext* phys;

};


#endif //N_BODY_SOLVER_H
