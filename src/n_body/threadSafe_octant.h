//
// Created by jackcamp on 2/24/19.
//

#ifndef N_BODY_THREADSAFE_OCTANT_H
#define N_BODY_THREADSAFE_OCTANT_H


#include <iostream>
#include <glm/glm.hpp>

#include "body.h"
#include "simulation.h"


/**
 * This octree is designed from the ground up with multi-threading in mind
 */
class threadSafe_octant {

public:

    /**
     * Constructor for a new octant
     * @param location The center of the octant as a vector
     * @param sideLength The length of the octant
     */
    threadSafe_octant(glm::vec3 location, float sideLength);

    /**
     * Standard destructor for the class
     */
    virtual ~threadSafe_octant() = default;

    /**
     * Adds a body to the octant by its parameters
     * @param newPosition the location of the body
     * @param newMass the mass of the body
     */
    void addBody(glm::vec3 newPosition, float newMass);

    /**
     * Applies gravity to a body based on the parameters of a simulation
     * @param theBody The body to apply the forces of gravity to
     * @param theSimulation the simulation in which calculations are defined
     */
    void applyGravityToBody(body *theBody, simulation *theSimulation);

    /**
     * Creates a string representation of the entire tree
     * @return a string describing the octant
     */
    std::string toString(int level = 0);


private:

    // Values used for gravity calculations

    /*Position and mass of the body or bodies contained by the node*/
    vec3 position;
    float mass;


    // Subdivisions of the node

    /*Whether or not the node is already divided*/
    bool divided = false;

    /*Three dimensional array of smart pointers to child trees*/
    shared_ptr<octant> children[2][2][2];


    // Metadata about the node

    /*Size of the node and position of its center*/
    float sideLength;
    vec3 octantLocation;


    /*Useful data for determining the ideal position of the next iteration's octree*/
    vec3 averagePosition = vec3(0, 0, 0); /*What the center of mass would be if all bodies had identical masses*/
    int numBodies;


    // Helper methods

    /*Splits the octant into subnodes*/
    void divide();

    /*Finds the appropriate child to add a body to*/
    shared_ptr<octant> getSubdivisionEnclosing(vec3 position);

};


#endif //N_BODY_THREADSAFE_OCTANT_H
