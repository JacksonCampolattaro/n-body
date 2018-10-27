//
// Created by jackcamp on 10/19/18.
//

#include "simulationState.h"
#include "viewport.h"

simulationState::simulationState(float gravitationalConstant, float timeInterval, int power) {

    this->gravitationalConstant = gravitationalConstant;
    this->timeInterval = timeInterval;
    this->power = power;

}

void simulationState::addBody(orbitalBody *newBody) {

    // Adds relationships to link the new body to each of the other bodies in the array.
    for (orbitalBody *theBody : bodies) {
        relationships.push_back(new relationship(theBody, newBody));
    }

    // Adds the new body to the collection
    bodies.push_back(newBody);
}

void simulationState::increment() {

    // Updates each Relationship
#pragma omp parallel for
    for (int i = 0; i < relationships.size(); ++i) {
        relationships[i]->applyGravity(timeInterval, gravitationalConstant, power);
    }

    // Updates each orbitalBody
#pragma omp parallel for
    for (int j = 0; j < bodies.size(); ++j) {
        bodies[j]->applyVelocity(timeInterval);
    }
}

void simulationState::draw() {

    // Updates each orbitalBody
    for (orbitalBody *b : bodies) {
        b->draw();
    }
}
