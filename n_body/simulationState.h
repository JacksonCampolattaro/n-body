//
// Created by jackcamp on 10/19/18.
//

#ifndef N_BODY_HEADLESS_SIMULATIONSTATE_H
#define N_BODY_HEADLESS_SIMULATIONSTATE_H

#include <vector>

#include "orbitalBody.h"
#include "relationship.h"

/**
 * Contains the current state of the simulation
 * and has methods for updating.
 */
class simulationState {

public:

    /**
     * Constructor for a simulation state
     */
    simulationState(float gravitationalConstant, float timeInterval, int power = 2) {

        this->gravitationalConstant = gravitationalConstant;
        this->timeInterval = timeInterval;

    }

    /**
     * Adds a pre-built body to the simulation
     *
     * @param newBody the body to be added.
     */
    void addBody(orbitalBody *newBody) {

        // Adds relationships to link the new body to each of the other bodies in the array.
        for (orbitalBody *theBody : bodies) {
            relationships.push_back(new relationship(theBody, newBody));
        }

        // Adds the new body to the collection
        bodies.push_back(newBody);
    }

    /**
     * Increments the simulation by applying physics to all bodies
     */
    void increment() {

        // Updates each bodyRelationship
        for (relationship *r : relationships) {

            r->applyGravity(timeInterval, gravitationalConstant);
        }

        // Updates each orbitalBody
        for (orbitalBody *b : bodies) {

            b->applyVelocity(timeInterval);
        }
    }

    /**
     * Tells each body to draw itself
     */
    void draw() {

        // Updates each orbitalBody
        for (orbitalBody *b : bodies) {

            b->draw();
        }
    }


private:

    // Simulation parameters
    float gravitationalConstant;
    float timeInterval;

    // Contains all bodies
    std::vector<orbitalBody *> bodies;

    // Contains all relationships between bodies
    std::vector<relationship *> relationships;

};


#endif //N_BODY_HEADLESS_SIMULATIONSTATE_H
