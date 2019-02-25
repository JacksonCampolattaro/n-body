//
// Created by jackcamp on 10/30/18.
//

#ifndef N_BODY_HEADLESS_OCTANT_H
#define N_BODY_HEADLESS_OCTANT_H

#include <glm/glm.hpp>
#include <boost/multi_array.hpp>
#include <tbb/tbb.h>

#include "body.h"
#include "relationship.h"

class simulation;

/**
 * My implementation of the Barnes-Hut algorithm
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
    virtual ~octant();

    /**
     * Adds a body to the octant by its parameters
     * @param newPosition the location of the body
     * @param newMass the mass of the body
     */
    void addBody(vec3 newPosition, float newMass);

    /**
     * Adds a list of bodies to the octant
     * @param newBodies the bodies to be added
     */
    void addBodies(std::vector<body*> newBodies);

    /**
     * Adds the list of bodies to the octant, dedicating one thread to each subtree
     * @param newBodies The list of bodies to be added
     */
    void eightThreadAdd(std::vector<body*> newBodies);

    /**
     * Processes a queue of bodies using only one thread
     * @param newBodies the bodies to be processed
     */
    void singleThreadProcess(tbb::concurrent_queue<body*> newBodies);

    /**
     * Adds a body to a queue to be added to the tree in a multi-threaded way
     * @param newPosition the location of the body
     * @param newMass the mass of the body
     */
    void enqueueBody(vec3 newPosition, float newMass);

    /**
     * Distributes the queue of bodies to the octant's subnodes
     */
    void processBodyQueue();

    /**
     * Creates a body that represents the center of mass of the region
     * @return body sum of contained bodies
     */
    void calculateCenterMass();

    /**
     * Applies all forces to the body
     * @param theBody The body to be affected
     * @param theta Constant determining accuracy, Lower --> More accurate
     */
    void applyGravity(body *inputBody, float theta, simulation *simulation);

    /**
     * Returns the requested subdivision of the octant
     * @return one of the eight octants contained by the parent
     */
    octant *getSubdivision(ivec3);

    /**
     * Takes a poll of the bodies
     * @return the number of bodies contained by the octant
     */
    int numBodies();

    /**
     * Creates a string representation of the entire tree
     * @return a string describing the octant
     */
    std::string toString(int level = 0);

    const vec3 &getPosition() const;

    float getMass() const;

private:

    // Location of the octant
    glm::vec3 location;

    // Bounds of the octant
    float sideLength;

    // Position and mass of the body or bodies contained by the node
    vec3 position;
    float mass;
    bool isLeaf = false;

    bool calculatedCOM = false;

    // A thread safe way to add and retrieve bodies to be processed
    tbb::concurrent_queue<vec3> positionsToBeProcessed;
    tbb::concurrent_queue<float> massesToBeProcessed;

    // 3d array of the subdivisions of this octant
    bool divided = false;
    std::shared_ptr<octant> subdivisions[2][2][2]; // No sub-nodes by default

    void divide();

    std::shared_ptr<octant> subdivisionEnclosing(vec3 position);

};


#endif //N_BODY_HEADLESS_OCTANT_H