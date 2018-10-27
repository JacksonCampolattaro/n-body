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

    // Updates each bodyRelationship
    for (relationship *r : relationships) {

        r->applyGravity(timeInterval, gravitationalConstant, power);
    }

    // Updates each orbitalBody
    for (orbitalBody *b : bodies) {

        b->applyVelocity(timeInterval);
    }
}

void simulationState::draw(viewport *client) {

    // Updates each orbitalBody
    for (orbitalBody *b : bodies) {
        b->draw(client);
    }
}
