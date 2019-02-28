//
// Created by jackcamp on 2/25/19.
//

#include "simulation.h"

#include <glm/glm.hpp>

#include "octant.h"

simulation::simulation() = default;

void simulation::addBody(body *body) {

    this->bodies.push_back(body);
}

simulation *simulation::setG(float G) {

    this->G = G;

    return this;
}

simulation *simulation::setPower(int Power) {

    this->Exponent = Power;

    return this;
}

simulation *simulation::setT(float T) {

    this->T = T;

    return this;
}

simulation *simulation::enableForceSoftening(bool enabled) {

    this->ForceSofteningEnabled = enabled;

    return this;
}

simulation *simulation::setMinimumT(float minimumT) {

    this->minimumT = minimumT;

    return this;
}

simulation *simulation::enableBarnesHut(bool enabled) {

    this->BarnesHutEnabled = enabled;

    return this;
}

simulation *simulation::setTheta(float Theta) {
    this->Theta = Theta;

    return this;
}

simulation *simulation::enableLeapfrog(bool enabled) {

    this->LeapFrogEnabled = enabled;

    return this;
}

simulation *simulation::enableThreading(bool enabled) {

    this->threadingEnabled = enabled;

    return this;
}

/*simulation *simulation::attachViewport(viewport *theViewport) {

    this->theViewport = theViewport;

    return this;
}*/



float simulation::getTheta() {

    return Theta;
}


void simulation::increment() {

    // Precalculations

    /*Adds the offset that turns euler into leapfrog integration*/
    if (LeapFrogEnabled) {

        T = T / 2.0f;
        calculateGravity();
        T = T * 2.0f;

        /*This ensures that the offset is only added the first time through the loop*/
        LeapFrogEnabled = false;
    }


    // Gravitational Calculations
    calculateGravity();


    // Updating velocities and positions
    #pragma omp parallel for if(threadingEnabled)
    for (int b = 0; b < bodies.size(); ++b) {

        bodies[b]->drift(T);
        bodies[b]->shiftBuffers();
    }


}

void simulation::applyGravityBetweenBodies(body *subject, glm::vec3 actorPosition, float actorMass) {

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
    glm::vec3 acceleration = force / subject->getMass();

    // Applying the acceleration to the body (v = at)
    // TODO When I add force softening / super-sampling, this is where it will be done
    subject->kick(acceleration * T);

}

void simulation::orbit(body *sunBody, body *satelliteBody) {

    // TODO This method needs improvement

    // Getting the distance between the bodies
    float distance = glm::distance(sunBody->getPosition(), satelliteBody->getPosition());

    // Calculating the necessary velocity to maintain orbit
    float orbitalVelocity = (float) sqrt((G * sunBody->getMass() / distance));

    // Setting the velocity
    // TODO ...in a direction perpendicular to the radial distance line?
    satelliteBody->setVelocity(orbitalVelocity * glm::normalize(satelliteBody->getVelocity()));
}

void simulation::calculateGravity() {


    if (BarnesHutEnabled) {

        // Uses the Barnes-Hut algorithm if it's enabled
        BarnesHutGravity();

    } else {

        // Otherwise calculates gravity using the naive strategy
        NaiveGravity();
    }
}

void simulation::NaiveGravity() {

    // Iterating through each combination of bodies
    #pragma omp parallel for if(threadingEnabled)
    for (int subject = 0; subject < bodies.size(); ++subject) {
        for (int actor = 0; actor < bodies.size(); ++actor) {

            // Getting this particular pair
            body *subjectBody = bodies[subject];
            body *actorBody = bodies[actor];

            // Skips this interaction if the subject is fixed or the actor is passive
            if (!subjectBody->isFixed() && !actorBody->isPassive() &&
                subjectBody->getPosition() != actorBody->getPosition()) {

                // Applies the force of the actor on the subject
                applyGravityBetweenBodies(subjectBody, actorBody->getPosition(), actorBody->getMass());
            }
        }
    }

}

void simulation::BarnesHutGravity() {

    // TODO Implementation using the rewritten octree

    // Creating the tree
    //auto octree = new octant(idealTreeCenterLocation, 10000);
    std::unique_ptr<octant> octree (std::make_unique<octant>(idealTreeCenterLocation, 1000000));

    // Populating the tree
    // TODO Once the octant is threadsafe, this will be possible to do in parallel
    //#pragma omp parallel for if(threadingEnabled)
    for (int b = 0; b < bodies.size(); ++b) {
        octree->addBody(bodies[b]->getPosition(), bodies[b]->getMass());
    }

    // Getting the center of mass and the ideal location for the next tree
    octree->getCenterOfMass();
    idealTreeCenterLocation = octree->getAveragePosition();

    // Doing gravitational calculations
    #pragma omp parallel for if(threadingEnabled)
    for (int b = 0; b < bodies.size(); ++b) {
        octree->applyGravityToBody(bodies[b], this);
    }
}
