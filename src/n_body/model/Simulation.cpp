//
// Created by jackcamp on 2/25/19.
//

#include "Simulation.h"

#include <glm/glm.hpp>

#include "calculation/BarnesHut/Octant.h"

Simulation::Simulation() = default;

void Simulation::addBody(Body *body) {

    this->bodies.push_back(body);
}

Simulation *Simulation::setG(float G) {

    this->G = G;

    return this;
}

Simulation *Simulation::setPower(int Power) {

    this->Exponent = Power;

    return this;
}

Simulation *Simulation::setT(float T) {

    this->T = T;

    return this;
}

Simulation *Simulation::enableForceSoftening(bool enabled) {

    this->ForceSofteningEnabled = enabled;

    return this;
}

Simulation *Simulation::setMinimumT(float minimumT) {

    this->minimumT = minimumT;

    return this;
}

Simulation *Simulation::enableBarnesHut(bool enabled) {

    this->BarnesHutEnabled = enabled;

    return this;
}

Simulation *Simulation::setTheta(float Theta) {
    this->Theta = Theta;

    return this;
}

Simulation *Simulation::enableLeapfrog(bool enabled) {

    this->LeapFrogEnabled = enabled;

    return this;
}

Simulation *Simulation::enableThreading(bool enabled) {

    this->threadingEnabled = enabled;

    return this;
}

/*Simulation *Simulation::attachViewport(Viewport *theViewport) {

    this->theViewport = theViewport;

    return this;
}*/



float Simulation::getTheta() {

    return Theta;
}


void Simulation::increment() {

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

void Simulation::applyGravityBetweenBodies(Body *subject, glm::vec3 actorPosition, float actorMass) {

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

    // Applying the acceleration to the Body (v = at)
    // TODO When I add force softening / super-sampling, this is where it will be done
    subject->kick(acceleration * T);

}

void Simulation::orbit(Body *sunBody, Body *satelliteBody) {

    // TODO This method needs improvement

    // Getting the distance between the bodies
    float distance = glm::distance(sunBody->getPosition(), satelliteBody->getPosition());

    // Calculating the necessary velocity to maintain orbit
    float orbitalVelocity = (float) sqrt((G * sunBody->getMass() / distance));

    // Setting the velocity
    // TODO ...in a direction perpendicular to the radial distance line?
    satelliteBody->setVelocity(orbitalVelocity * glm::normalize(satelliteBody->getVelocity()));
}

void Simulation::calculateGravity() {


    if (BarnesHutEnabled) {

        // Uses the Barnes-Hut algorithm if it's enabled
        BarnesHutGravity();

    } else {

        // Otherwise calculates gravity using the naive strategy
        NaiveGravity();
    }
}

void Simulation::NaiveGravity() {

    // Iterating through each combination of bodies
    #pragma omp parallel for if(threadingEnabled)
    for (int subject = 0; subject < bodies.size(); ++subject) {
        for (int actor = 0; actor < bodies.size(); ++actor) {

            // Getting this particular pair
            Body *subjectBody = bodies[subject];
            Body *actorBody = bodies[actor];

            // Skips this interaction if the subject is fixed or the actor is passive
            if (!subjectBody->isFixed() && !actorBody->isPassive() &&
                subjectBody->getPosition() != actorBody->getPosition()) {

                // Applies the force of the actor on the subject
                applyGravityBetweenBodies(subjectBody, actorBody->getPosition(), actorBody->getMass());
            }
        }
    }

}

void Simulation::BarnesHutGravity() {

    // TODO Implementation using the rewritten octree

    // Creating the tree
    std::unique_ptr<Octant> octree (std::make_unique<Octant>(idealTreeCenterLocation, 1000000));

    // Populating the tree
    // TODO Once the Octant is threadsafe, this will be possible to do in parallel
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

