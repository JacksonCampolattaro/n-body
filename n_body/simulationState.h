//
// Created by jackcamp on 10/19/18.
//

#ifndef N_BODY_HEADLESS_SIMULATIONSTATE_H
#define N_BODY_HEADLESS_SIMULATIONSTATE_H

#include <vector>

#include "body.h"
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
    simulationState(float gravitationalConstant, float timeInterval, int power = 3) {

        this->gravitationalConstant = gravitationalConstant;
        this->timeInterval = timeInterval;

    }

    /**
     * Adds a pre-built body to the simulation
     *
     * @param newBody the body to be added.
     */
    void addBody(body newBody) {

        // Adds a pointer to the new body to the bodies collection
        bodies.push_back(&newBody);
    }

    /**
     * Increments the simulation by applying physics to all bodies
     */
    void increment() {

        // Updates each relationship
        for (relationship *r : relationships) {

            r->applyGravity(timeInterval, gravitationalConstant);
        }

        // Updates each body
        for (body *b : bodies) {

            b->applyVelocity(timeInterval);
        }
    }


private:

    // Simulation parameters
    float gravitationalConstant;
    float timeInterval;

    // Contains all bodies
    std::vector<body *> bodies;

    // Contains all relationships between bodies
    std::vector<relationship *> relationships;

};


#endif //N_BODY_HEADLESS_SIMULATIONSTATE_H
