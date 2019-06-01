//
// Created by jackcamp on 5/28/19.
//

#include "refactoredSolver.h"

void refactoredSolver::build(std::vector<Body *> bodies, PhysicsContext *physicsContext) {

    // Sets the physicsContext context
    this->physicsContext = physicsContext;

    // Adds only the active bodies
    for (int b = 0; b < bodies.size(); ++b) {
        if (!bodies[b]->isPassive()) {
            activeBodies.push_back(bodies[b]);
        }
    }
}

refactoredSolver *refactoredSolver::enableThreading(bool enabled) {

    threadingEnabled = enabled;
    return this;
}
