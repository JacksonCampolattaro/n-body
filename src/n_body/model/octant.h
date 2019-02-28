//
// Created by jackcamp on 2/24/19.
//

#ifndef N_BODY_OCTANT_H
#define N_BODY_OCTANT_H


#include "body.h" // I pass in a body when I want to calculate the forces on it
#include "simulation.h" // The gravity method in the simulation does the actual calculations

#include <glm/glm.hpp> // Used for storing locations in cartesian space
#include <atomic> // Used for atomic variables
#include <mutex> // Used for the call_once flag and lambda expression
#include <memory> // I used smart pointers to resolve a memory leak
#include <string> // Used by the toString method


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
class octant {

public:

    /**
     * Constructor for a new octant
     * @param location The center of the octant as a vector
     * @param sideLength The length of the octant
     */
    octant(glm::vec3 location, float sideLength);

    /**
     * Standard destructor for the class
     */
    virtual ~octant() = default;

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
     * @param theSim the simulation in which calculations are defined
     */
    void applyGravityToBody(body *theBody, simulation *theSim);

    /**
     * Getter for the number of bodies contained by the octant
     * @return One if leaf node, otherwise the sum of the bodies in all child nodes
     */
    int getNumBodies();

    /**
     * Getter for the center of mass, which calculates it if it isn't already generated
     * @return The center of mass as a vector
     */
    glm::vec3 getCenterOfMass();

    /**
     * Getter for the average position of the bodies in the tree, which calculates it if necessary
     * @return The ideal position of the next iteration's octree
     */
    glm::vec3 getAveragePosition();

    /**
     * Creates a string representation of the entire tree
     * @return a string describing the octant
     */
    std::string toString(int level = 0);

private:

    // Values used for gravity calculations

    /*Position and mass of the body or bodies contained by the node*/
    glm::vec3 centerOfMass;
    float totalMass;

    /*By keeping track of this, center of mass doesn't have to be recalculated every time a body is added*/
    std::atomic_bool validCenterOfMass = {true}; /*The COM will naturally be valid when this is a leaf*/


    // Subdivisions of the node

    /*Whether or not the node is already divided, atomic for thread safety reasons*/
    std::atomic_bool divided = {false};
    std::once_flag split;

    /*Three dimensional array of smart pointers to child trees*/
    std::shared_ptr<octant> children[2][2][2];


    // Metadata about the node

    /*Whether or not the node isn't empty, atomic for thread safety reasons*/
    std::atomic_bool initialized = {false};

    /*Size of the node and position of its center*/
    float sideLength;
    glm::vec3 octantLocation;

    /*Useful data for determining the ideal position of the next iteration's octree*/
    glm::vec3 averagePosition = glm::vec3(0, 0,
                                          0); /*What the center of mass would be if all bodies had identical masses*/
    std::atomic_bool validAveragePosition = {true};

    /*Total number of bodies contained by the octant or its children*/
    int numBodies;


    // Helper methods

    /*Splits the octant into subnodes*/
    void divide();

    /*Finds the appropriate child to add a body to*/
    std::shared_ptr<octant> getSubdivisionEnclosing(glm::vec3 position);

};


#endif //N_BODY_OCTANT_H
