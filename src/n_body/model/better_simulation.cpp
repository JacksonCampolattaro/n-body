//
// Created by jackcamp on 2/25/19.
//

#include "better_simulation.h"



better_simulation::better_simulation() = default;

void better_simulation::addBody(better_body *body) {

    this->bodies.push_back(body);
}

better_simulation *better_simulation::setG(float G) {

    this->G = G;

    return this;
}

better_simulation *better_simulation::setPower(int Power) {

    this->Exponent = Power;

    return this;
}

better_simulation *better_simulation::setT(float T) {

    this->T = T;

    return this;
}

better_simulation *better_simulation::enableForceSoftening(bool enabled) {

    this->ForceSofteningEnabled = enabled;

    return this;
}

better_simulation *better_simulation::setMinimumT(float minimumT) {

    this->minimumT = minimumT;

    return this;
}

better_simulation *better_simulation::enableBarnesHut(bool enabled) {

    this->BarnesHutEnabled = enabled;

    return this;
}

better_simulation *better_simulation::setTheta(float Theta) {
    this->Theta = Theta;

    return this;
}

better_simulation *better_simulation::enableLeapfrog() {

    this->LeapFrogEnabled = true;

    return this;
}

better_simulation *better_simulation::attachViewport(better_viewport *viewport) {

    this->viewport = viewport;

    return this;
}



float better_simulation::getTheta() {

    return Theta;
}


void better_simulation::increment() {

    tracker::instance()->markFrameCompleted(); /*Marks the completion of the previous frame, including rendering*/
    tracker::instance()->markCalculationsStart();

    // Precalculations

    /*Adds the offset that turns euler into leapfrog integration*/
    if (LeapFrogEnabled) {

        // TODO Physics calculations will go here

        /*This ensures that the offset is only added the first time through the loop*/
        LeapFrogEnabled = false;
    }


    // Gravitational Calculations
    calculateGravity();
    tracker::instance()->markGravityCalculated();

    // Updating velocities and positions
    for (int b = 0; b < bodies.size(); ++b) {
        bodies[b]->update();
    }


}

void better_simulation::applyGravityBetweenBodies(better_body *subject, glm::vec3 actorPosition, float actorMass) {

    // Added on the bottom of the equation to prevent infinities
    // TODO Should this be a model parameter?
    float infinityPrevention = 0.0001;

    // Calculating the force of gravity as a scalar
    float forceOfGravity =
            (G * subject->getMass() * actorMass) /
            ((float) pow(glm::distance(subject->getPosition(), actorPosition), Exponent) + infinityPrevention);

    // Giving the force direction
    glm::vec3 force = forceOfGravity * glm::normalize(actorPosition - subject->getPosition());

    // Getting acceleration (a = f/m)
    vec3 acceleration = force / subject->getMass();

    // Applying the acceleration to the body (v = at)
    // TODO When I add force softening / super-sampling, this is where it will be done
    subject->addVelocity(acceleration * T);

}

void better_simulation::orbit(better_body *sunBody, better_body *satelliteBody) {

    // TODO This method needs improvement

    // Getting the distance between the bodies
    float distance = glm::distance(sunBody->getPosition(), satelliteBody->getPosition());

    // Calculating the necessary velocity to maintain orbit
    float orbitalVelocity = sqrt((G * sunBody->getMass() / distance));

    // Setting the velocity
    // TODO ...in a direction perpendicular to the radial distance line?
    satelliteBody->setVelocity(orbitalVelocity * glm::normalize(satelliteBody->getVelocity()));
}

void better_simulation::calculateGravity() {


    if (BarnesHutEnabled) {

        // Uses the Barnes-Hut algorithm if it's enabled
        BarnesHutGravity();

    } else {

        // Otherwise calculates gravity using the naive strategy
        NaiveGravity();
    }
}

void better_simulation::NaiveGravity() {

    // Iterating through each combination of bodies
    for (int subject = 0; subject < bodies.size(); ++subject) {
        for (int actor = 0; actor < bodies.size(); ++actor) {

            // Getting this particular pair
            better_body *subjectBody = bodies[subject];
            better_body *actorBody = bodies[actor];

            // Skips this interaction if the subject is fixed or the actor is passive
            if (!subjectBody->isFixed() && !actorBody->isPassive()) {

                // Applies the force of the actor on the subject
                applyGravityBetweenBodies(subjectBody, actorBody->getPosition(), actorBody->getMass());
            }
        }
    }

}

void better_simulation::BarnesHutGravity() {

    // TODO Implementation using the rewritten octree

    // Creating the tree

    // Populating the tree

    // Getting the ideal location for the next tree

    // Doing gravitational calculations
}

