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
class simulation {

public:

    /**
     * Constructor for a simulation state
     */
    simulation(float gravitationalConstant, float timeInterval, int power = 2);

    /**
     * Adds a pre-built body to the simulation
     *
     * @param newBody the body to be added.
     */
    void addBody(body *newBody);

    /**
     * Increments the simulation by applying physics to all bodies
     */
    void increment();

    /**
     * Tells each body to draw itself
     */
    void draw();

    /**
     * Tells each body to announce its properties in the console
     */
    void soundOff() {

        // Updates each body
        for (body *b : bodies) {

            b->soundOff();
        }
    }

    /**
     * Allows the user to force a body to orbit another by changing its velocity
     * @param sunBody The body to orbit
     * @param satelliteBody The body orbiting
     */
    void orbit(body *sunBody, body *satelliteBody);


private:

    // Simulation parameters
    float gravitationalConstant;
    float timeInterval;
    int power;

    // Contains all bodies
    std::vector<body *> bodies;

    // Contains all relationships between bodies
    std::vector<relationship *> relationships;

};


#endif //N_BODY_HEADLESS_SIMULATIONSTATE_H
