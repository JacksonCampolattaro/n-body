//
// Created by jackcamp on 4/10/19.
//

#include "Model.h"

Model::Model(PhysicsContext *physics, Solver *solver) {

    this->physics = physics;
    this->solver = solver;
}

void Model::preCalculate(std::vector<Body *> bodies) {

}

void Model::increment(std::vector<Body *> bodies) {

    solver->solve(bodies, physics);

    // Updating velocities and positions
    #pragma omp parallel for
    for (int b = 0; b < bodies.size(); ++b) {

        bodies[b]->drift(physics->getT());
        bodies[b]->shiftBuffers();
    }

}