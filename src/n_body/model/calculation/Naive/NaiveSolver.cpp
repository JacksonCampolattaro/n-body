//
// Created by jackcamp on 4/10/19.
//

#include "NaiveSolver.h"
#include "../../PhysicsContext.h"

void NaiveSolver::solve(std::vector<Body> *bodies, PhysicsContext *phys) {

    // Iterating through each combination of bodies
    #pragma omp parallel for if(threadingEnabled)
    for (int subject = 0; subject < bodies->size(); ++subject) {
        for (int actor = 0; actor < bodies->size(); ++actor) {

            // Getting this particular pair
            Body subjectBody = (*bodies)[subject];
            Body actorBody = (*bodies)[actor];

            glm::vec3 actorPosition = actorBody.getPosition();
            float actorMass = actorBody.getMass();

            // Skips this interaction if the subject is fixed or the actor is passive, also prevents self-interaction
            if (!subjectBody.isFixed() && !actorBody.isPassive() &&
                subjectBody.getPosition() != actorPosition) {

                // Applies the force of the actor on the subject
                phys->applyGravityBetweenBodies(&(*bodies)[subject], actorPosition, actorMass);
            }
        }
    }

    ///signal_complete().emit();
}
