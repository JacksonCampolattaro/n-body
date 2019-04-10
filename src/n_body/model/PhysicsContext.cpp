//
// Created by jackcamp on 4/5/19.
//

#include "PhysicsContext.h"

PhysicsContext *PhysicsContext::setG(float G) {

    this->G = G;

    return this;
}

PhysicsContext *PhysicsContext::setPower(int Power) {

    this->Exponent = Power;

    return this;
}

PhysicsContext *PhysicsContext::setT(float T) {

    this->T = T;

    return this;
}

PhysicsContext *PhysicsContext::enableForceSoftening(bool enabled) {

    this->ForceSofteningEnabled = enabled;

    return this;
}

PhysicsContext *PhysicsContext::setMinimumT(float minimumT) {

    this->minimumT = minimumT;

    return this;
}

void PhysicsContext::applyGravityBetweenBodies(Body *subject, glm::vec3 actorPosition, float actorMass) {

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

void PhysicsContext::orbit(Body *sunBody, Body *satelliteBody) {

    // TODO This method needs improvement

    // Getting the distance between the bodies
    float distance = glm::distance(sunBody->getPosition(), satelliteBody->getPosition());

    // Calculating the necessary velocity to maintain orbit
    float orbitalVelocity = (float) sqrt((G * sunBody->getMass() / distance));

    // Setting the velocity
    // TODO ...in a direction perpendicular to the radial distance line?
    satelliteBody->setVelocity(orbitalVelocity * glm::normalize(satelliteBody->getVelocity()));
}
