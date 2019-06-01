//
// Created by jackcamp on 5/28/19.
//

#ifndef N_BODY_REFACTOREDSOLVER_H
#define N_BODY_REFACTOREDSOLVER_H

#include "../Body.h" // The Solver uses bodies
#include "../PhysicsContext.h" // The solver affects the bodies according to the physicsContext it's given

#include <vector> // Used to handle the list of bodies in the Simulation


class refactoredSolver {

public:

    /**
     * Prepares the solver to apply forces to bodies
     * In a Barnes-Hut solver, this entails building the tree
     * @param bodies The list of bodies involved in the calculation
     * @param physicsContext The physics context used by the simulation
     */
    virtual void build(std::vector<Body *> bodies, PhysicsContext *physicsContext);


    /**
     * Uses the solver to apply forces to a body
     * @param body The body to apply forces to
     */
    virtual void kick(Body *body) = 0;


    /**
     * Used to enable multithreaded workloads via openmp
     * @param enabled whether or not multithreading should be allowed
     * @return This solver, for use in chaining named parameters.
     */
    refactoredSolver *enableThreading(bool enabled = true);


protected:

    /*Flag enabling multithreading of workloads*/
    bool threadingEnabled = false;


    /*PhysicsContext used by the simulation*/
    PhysicsContext *physicsContext = new PhysicsContext;


    /*List of bodies capable of applying force to other bodies*/
    std::vector<Body *> activeBodies;
};


#endif //N_BODY_REFACTOREDSOLVER_H
