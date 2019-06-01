//
// Created by jackcamp on 5/30/19.
//

#include "refactoredNaiveSolver.h"

void refactoredNaiveSolver::build(std::vector<Body *> bodies, PhysicsContext *physicsContext) {

    // Naive solver doesn't have to do anything special when preparing itself
    /* This calls the function defined in the superclass */
    refactoredSolver::build(bodies, physicsContext);
}

void refactoredNaiveSolver::kick(Body *subjectBody) {

    // Iterates through each of the other bodies, and applies that body's force to this one
    #pragma omp parallel for if(threadingEnabled)
    for (int actorIndex = 0; actorIndex < activeBodies.size(); ++actorIndex) {

        // Retrieves information about the actor
        auto actorBody = activeBodies[actorIndex];
        auto actorPosition = actorBody->getPosition();
        auto actorMass = actorBody->getMass();

        // Only try to apply forces if the subject isn't fixed and the actor isn't in the same place
        if (!subjectBody->isFixed() && subjectBody->getPosition() != actorPosition) {
            physicsContext->applyGravityBetweenBodies(subjectBody, actorPosition, actorMass);
        }
    }
}
