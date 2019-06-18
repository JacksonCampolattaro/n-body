//
// Created by jackcamp on 4/10/19.
//

#include <iostream>
#include "Model.h"

Model::Model(PhysicsContext *physics, Solver *solver) {

    this->physicsContext = physics;

    this->solver = solver;

    /*auto BHSolver = new refactoredBarnesHutSolver;
    BHSolver->setTheta(0.9);
    this->rSolver = BHSolver;*/
}

void Model::preCalculate(std::vector<Body *> bodies) {

    physicsContext->setT(physicsContext->getT() / 2.0f);
    solver->solve(bodies, physicsContext);
    physicsContext->setT(physicsContext->getT() * 2.0f);

}

void Model::increment(std::vector<Body *> bodies) {

    solver->solve(bodies, physicsContext);

    // Updating velocities
    #pragma omp parallel for
    for (int b = 0; b < bodies.size(); ++b) {

        bodies[b]->drift(physicsContext->getT());
    }

    /*// Preparing the solver
    rSolver->build(bodies, physicsContext);

    // Acting on each body
    #pragma omp parallel for
    for (int b = 0; b < bodies.size(); ++b) {

        rSolver->kick(bodies[b]);
    }

    // Moving each body
    #pragma omp parallel for
    for (int b = 0; b < bodies.size(); ++b) {

        bodies[b]->drift(physicsContext->getT());
    }*/

}

void Model::increment(std::vector<Body> *bodies) {

    // Updating velocities ("kick")
    solver->solve(bodies, physicsContext);

    // Updating positions ("drift")
    #pragma omp parallel for
    for (int b = 0; b < bodies->size(); ++b) {

        (*bodies)[b].drift(physicsContext->getT());
    }
}
