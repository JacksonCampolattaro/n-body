//
// Created by jackcamp on 10/30/18.
//

#ifndef N_BODY_HEADLESS_OCTANT_H
#define N_BODY_HEADLESS_OCTANT_H

#include <glm/glm.hpp>
#include <boost/multi_array.hpp>
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
     * Empties the octant and resets
     */
    void clear();

    /**
     * Adds a body to the octant
     * @param newBody The body to be added
     *//*
    void addBody(body *newBody);*/

    /**
     * Adds a body to the octant by its parameters
     * @param newPosition the location of the body
     * @param newMass the mass of the body
     */
    void addBody(vec3 newPosition, float newMass);

    /**
     * Creates a body that represents the center of mass of the region
     * @return body sum of contained bodies
     */
    void calculateCenterMass();

    /**
     * Returns the body that represents the center of mass of the region
     * @return body sum of contained bodies
     */
    body *getCenterMass();

    /**
     * Pairs the body with all the masses acting on it
     * @param theBody The body to be affected
     * @param theta Constant determining accuracy, Lower --> More accurate
     * @return the list of all relationships affecting the body
     */
    std::vector<relationship *> getRelationships(body *inputBody, float theta);

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

private:

    // Location of the octant
    glm::vec3 location;

    // Bounds of the octant
    float sideLength;

    // The body contained if this is the lowest node
    //std::unique_ptr<body> theBody;
    //body *theBody = nullptr;
    bool isLeaf = false;
    vec3 position;
    float mass;
public:
    const vec3 &getPosition() const;

    float getMass() const;

private:

    bool calculatedCOM = false;

    // 3d array of the subdivisions of this octant
    bool divided = false;
    std::shared_ptr<octant> subdivisions[2][2][2]; // No sub-nodes by default

    void divide();

    std::shared_ptr<octant> subdivisionEnclosing(body * b);

    std::shared_ptr<octant> subdivisionEnclosing(vec3 position);

};


#endif //N_BODY_HEADLESS_OCTANT_H