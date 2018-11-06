//
// Created by jackcamp on 10/19/18.
//

#include "simulation.h"
#include "viewport.h"

simulation::simulation(float gravitationalConstant, float timeInterval, int power) {

    this->gravitationalConstant = gravitationalConstant;
    this->timeInterval = timeInterval;
    this->power = power;

}

void simulation::addBody(body *newBody) {

    // Adds relationships to link the new body to each of the other bodies in the array.
    for (body *theBody : bodies) {
        relationships.push_back(new relationship(theBody, newBody));
    }

    // Adds the new body to the collection
    bodies.push_back(newBody);
}

void simulation::increment() {

    // Updates each Relationship
#pragma omp parallel for
    for (int i = 0; i < relationships.size(); ++i) {
        relationships[i]->applyGravity(timeInterval, gravitationalConstant, power);
    }

    // Updates each body
#pragma omp parallel for
    for (int j = 0; j < bodies.size(); ++j) {
        bodies[j]->applyVelocity(timeInterval);
    }
}

void simulation::draw() {

    // Updates each body
    for (body *b : bodies) {
        b->draw();
    }
}

void simulation::orbit(body *sunBody, body *satelliteBody) {

    // Calculating the necessary velocity
    float distance = glm::distance(sunBody->getPosition(), satelliteBody->getPosition());
    float orbitalVelocity = sqrt((gravitationalConstant * sunBody->getMass() / distance));
    satelliteBody->setVelocity(orbitalVelocity * glm::normalize(satelliteBody->getVelocity()));
}

void simulation::setGravitationalConstant(float gravitationalConstant) {
    simulation::gravitationalConstant = gravitationalConstant;
}

void simulation::setTimeInterval(float timeInterval) {
    simulation::timeInterval = timeInterval;
}

void simulation::setPower(int power) {
    simulation::power = power;
}
