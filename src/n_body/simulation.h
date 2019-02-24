//
// Created by jackcamp on 10/19/18.
//

#ifndef N_BODY_HEADLESS_SIMULATIONSTATE_H
#define N_BODY_HEADLESS_SIMULATIONSTATE_H

#include <vector>

#include "body.h"
#include "relationship.h"
#include "octant.h"

/**
 * Contains the current state of the simulation
 * and has methods for updating.
 */
class simulation {

public:

    /**
     * Constructor for a simulation state
     */
    simulation(float gravitationalConstant, float timeInterval, int power = 2, float theta = 0.8);

    /**
     * Adds a pre-built body to the simulation
     *
     * @param newBody the body to be added.
     */
    void addBody(body *newBody);

    /**
     * Calculates the first velocities with a half time-step.
     * This is necessary for leapfrog integration.
     * Should make the program more accurate than when using euler integration.
     */
    void preCalculate();

    /**
     * Increments the simulation by applying physics to all bodies
     */
    void increment();

    /**
     * Applies the gravity on the first body from the second and changes the velocity accordingly
     *
     * @param passive the body being affected
     * @param activePosition the position of the body creating the force
     * @param activePosition the mass of the body creating the force
     */
    void applyGravity(body *passive, vec3 activePosition, float activeMass);

    /**
     * Tells each body to draw itself
     */
    void draw();

    /**
     * Gets the number of bodies in the simulation
     * @return the number of bodies as an integer
     */
    int getNumBodies();

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

public:

    void setGravitationalConstant(float gravitationalConstant);

    void setTimeInterval(float timeInterval);

    void setPower(int power);

private:
    float timeInterval;
    int power;
    float theta;

    // Contains all bodies
    std::vector<body *> bodies;
    int numBodies;

    // Contains all relationships between bodies
    std::vector<relationship *> relationships;

    // Root octant used for Barnes-Hut optimization
    //octant *octree = new octant(vec3(0, 0, 0), 100000);
};


#endif //N_BODY_HEADLESS_SIMULATIONSTATE_H
