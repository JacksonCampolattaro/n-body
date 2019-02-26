//
// Created by jackcamp on 2/24/19.
//

#ifndef N_BODY_THREADSAFE_OCTANT_H
#define N_BODY_THREADSAFE_OCTANT_H

#include "../simulation.h"

#include <iostream>
#include <glm/glm.hpp>
#include <memory>
#include <atomic>
#include <mutex>


#include "../body.h"



/**
 * This octree is designed from the ground up with multi-threading in mind
 *
 * The goal is to make every method thread safe, using no locking
 * This is achieved by using atomic initialization markers,
 * so that certain processes can only ever be called once per instance of an octant
 *
 * I'm also trying to streamline the class by combining methods that are always used together
 * For example, the center of mass is calculated the first time it needs to be retrieved
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
     * This is used for the creation of the tree
     * @param newPosition the location of the body
     * @param newMass the mass of the body
     */
    void addBody(glm::vec3 newPosition, float newMass);

    /**
     * Applies gravity to a body based on the parameters of a simulation
     * This is used for the traversal of the tree
     * @param theBody The body to apply the forces of gravity to
     * @param theSimulation the simulation in which calculations are defined
     */
    void applyGravityToBody(body *theBody, simulation *theSimulation);

    /**
     * Getter for the number of bodies contained by the octant
     * @return One if leaf node, otherwise the sum of the bodies in all child nodes
     */
    int getNumBodies();

    /**
     * Getter for the center of mass, which calculates it if it isn't already generated
     * @return The center of mass as a vector
     */
    vec3 getCenterOfMass();

    /**
     * Getter for the average position of the bodies in the tree, which calculates it if necessary
     * @return The ideal position of the next iteration's octree
     */
    vec3 getAveragePosition();

    /**
     * Creates a string representation of the entire tree
     * @return a string describing the octant
     */
    std::string toString(int level = 0);

private:

    // Values used for gravity calculations

    /*Position and mass of the body or bodies contained by the node*/
    vec3 centerOfMass;
    float totalMass;

    /*By keeping track of this, center of mass doesn't have to be recalculated every time a body is added*/
    atomic_bool validCenterOfMass = {true}; /*The COM will naturally be valid when this is a leaf*/


    // Subdivisions of the node

    /*Whether or not the node is already divided, atomic for thread safety reasons*/
    atomic_bool divided = {false};
    once_flag split;

    /*Three dimensional array of smart pointers to child trees*/
    std::shared_ptr<threadSafe_octant> children[2][2][2];


    // Metadata about the node

    /*Whether or not the node isn't empty, atomic for thread safety reasons*/
    atomic_bool initialized = {false};

    /*Size of the node and position of its center*/
    float sideLength;
    vec3 octantLocation;

    /*Useful data for determining the ideal position of the next iteration's octree*/
    vec3 averagePosition = vec3(0, 0, 0); /*What the center of mass would be if all bodies had identical masses*/
    atomic_bool validAveragePosition = {true};

    /*Total number of bodies contained by the octant or its children*/
    int numBodies;


    // Helper methods

    /*Splits the octant into subnodes*/
    void divide();

    /*Finds the appropriate child to add a body to*/
    shared_ptr<threadSafe_octant> getSubdivisionEnclosing(vec3 position);

};


#endif //N_BODY_THREADSAFE_OCTANT_H
