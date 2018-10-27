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
    simulationState(float gravitationalConstant, float timeInterval, int power = 2);

    /**
     * Adds a pre-built body to the simulation
     *
     * @param newBody the body to be added.
     */
    void addBody(orbitalBody *newBody);

    /**
     * Increments the simulation by applying physics to all bodies
     */
    void increment();

    /**
     * Tells each body to draw itself
     */
    void draw(viewport *client);

    /**
     * Tells each body to announce its properties in the console
     */
    void soundOff() {

        // Updates each orbitalBody
        for (orbitalBody *b : bodies) {

            b->soundOff();
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
