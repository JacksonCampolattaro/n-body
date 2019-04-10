//
// Created by jackcamp on 2/25/19.
//

#ifndef N_BODY_SIMULATION_H
#define N_BODY_SIMULATION_H

#include "Body.h" // The Simulation contains bodies
#include "../Tracker.h" // Used to report performance to the Tracker
//#include "../interface/Viewport.h" // Used to keep a reference to the program's Viewport

#include <vector> // Used to hold the list of bodies in the Simulation

/**
 * A more streamlined simulation class
 * I'm using the fluent interface paradigm to make it simpler to construct.
 */
class Simulation {

public:

    // Constructor

    /**
     * Constructor for the simulation, every parameter starts with a default
     */
    Simulation();

    /**
     * Adds a new body to the simulation
     * @param body the body to be included
     */
    void addBody(Body* body);


    // Setters (with support for chaining)

    /**
     * Sets the constant of gravity
     * @param G The constant 'G' as a float
     * @return This simulation, for use in chaining named parameters.
     */
    Simulation *setG(float G);

    /**
     * Sets the power distance is raised to when calculating the force of gravity
     * @param Power the new power of the simulation
     * @return This simulation, for use in chaining named parameters.
     */
    Simulation *setPower(int Power);

    /**
     * Sets the time interval of the simulation
     * @param T Unit-less time interval
     * @return This simulation, for use in chaining named parameters.
     */
    Simulation *setT(float T);

    /**
     * Selects whether Force Softening is enabled, off by default
     * @return This simulation, for use in chaining named parameters.
     */
    Simulation *enableForceSoftening(bool enabled = true);

    /**
     * Sets the shortest time interval to use
     * @param minimumT The smallest time granularity allowed when super-sampling is enabled
     * @return This simulation, for use in chaining named parameters.
     */
    Simulation *setMinimumT(float minimumT);

    /**
     * Selects whether Barnes-Hut optimizations is enabled, on by default
     * @return This simulation, for use in chaining named parameters.
     */
    Simulation *enableBarnesHut(bool enabled = true);

    /**
     * Sets the accuracy of the simulation (lower is more accurate, 0 is naive)
     * @param Theta Ratio defining how far any group of bodies must be before they can be grouped together
     * @return This simulation, for use in chaining named parameters.
     */
    Simulation *setTheta(float Theta);

    /**
     * Leapfrog approximation starts the first increment with a velocity pre-calculation on a half time-step
     * @return This simulation, for use in chaining named parameters.
     */
    Simulation *enableLeapfrog(bool enabled = true);

    /**
     * Used to enable multithreaded workloads via openmp
     * @param enabled whether multithreading is allowed
     * @return This simulation, for use in chaining named parameters.
     */
    Simulation *enableThreading(bool enabled = true);

    /**
     * Links the viewport to be used to draw bodies to the screen
     * @param theViewport a pointer to the viewport
     * @return This simulation, for use in chaining named parameters.
     */
    //Simulation *attachViewport(Viewport *theViewport);


    // Getters

    /**
     * Used by the octree to determine whether a group is far enough away to use the center of mass
     * @return the constant Theta
     */
    float getTheta();


    // Physics methods

    /**
     * Calculates the next time step of the simulation, by calculating forces on all bodies.
     */
    void increment();

    /**
     * Applies the gravity on the first body from the second and changes the velocity accordingly
     * @param subject the body being affected
     * @param actorPosition the position of the body creating the force
     * @param actorPosition the mass of the body creating the force
     */
    void applyGravityBetweenBodies(Body *subject, glm::vec3 actorPosition, float actorMass);


    // User facing tools

    /**
     * Allows the user to force a body to orbit another by changing its velocity
     * @param sunBody The body to orbit
     * @param satelliteBody The body orbiting
     */
    void orbit(Body *sunBody, Body *satelliteBody);

private:

    // Constants defining forces

    /*The gravitational constant (Higher values correspond to stronger gravity*/
    float G = 1.0;

    /*The power distance is raised to when calculating the force of gravity*/
    int Exponent = 2;


    // Constants defining accuracy

    /*The time interval that forces are calculated with respect to*/
    float T = 1.0;

    /*Flag for whether or not super-sampling of forces should be used in cases of extreme acceleration*/
    // TODO Not yet implemented
    bool ForceSofteningEnabled = false;
    /*The smallest time granularity allowed when super-sampling is enabled*/
    float minimumT = 0.001;

    /*Flag for whether or not the barnes-hut tree should be used*/
    bool BarnesHutEnabled = false;
    /*Ratio defining how far any group of bodies must be before they can be grouped together*/
    float Theta = 0.8; /*A theta value of 0 degenerates to the naive algorithm*/

    /*Flag that tells the increment function to do a velocity pre-calculation on a half time-step*/
    bool LeapFrogEnabled = false; /*Set back to false immediately after the offset is added*/

    /*Flag telling the Simulation to account for collisions between bodies*/
    // TODO Not yet implemented
    bool CollisionsEnabled = false;

    /*Flag enabling multithreading of workloads*/
    bool threadingEnabled = false;


    // Values used as a part of the model

    /*Filled with bodies before the start of the model, bodies are passed by reference and not removed*/
    std::vector<Body *> bodies;

    /*Best position for the center of the octree, starts at <0, 0, 0>, but changes each cycle*/
    glm::vec3 idealTreeCenterLocation = glm::vec3(0, 0, 0); /*Placed to divide bodies evenly between its subnodes*/


    // Graphics

    /*The Viewport used to draw bodies to the screen*/
    //Viewport *theViewport;


    // Private helper methods

    /**
     * Selects which algorithm to use, and calculates gravity on all bodies
     */
    void calculateGravity();

    /**
     * Applies gravity to all bodies using the naive algorithm: direct interaction between all bodies.
     */
    void NaiveGravity();

    /**
     * Applies gravity to all bodies using the Barnes-Hut algorithm: octree center of mass method
     */
    void BarnesHutGravity();

};


#endif //N_BODY_SIMULATION_H
