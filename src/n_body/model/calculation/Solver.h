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

    /**
     * Used to enable multithreaded workloads via openmp
     * @param enabled whether or not multithreading should be allowed
     * @return This solver, for use in chaining named parameters.
     */
    Solver *enableThreading(bool enabled = true);

    /**
     * Calculates forces between bodies and applies them to each body
     * @param bodies the list of bodies to perform calculations on
     */
    virtual void solve(std::vector<Body *> bodies, PhysicsContext *phys) = 0;

protected:

    /*Flag enabling multithreading of workloads*/
    bool threadingEnabled = false;

};


#endif //N_BODY_SOLVER_H
