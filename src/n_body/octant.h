//
// Created by jackcamp on 10/30/18.
//

#ifndef N_BODY_HEADLESS_OCTANT_H
#define N_BODY_HEADLESS_OCTANT_H

#include <glm/glm.hpp>
#include "body.h"
#include "relationship.h"


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
     * Adds a body to the octant
     * @param newBody The body to be added
     */
    void addBody(body *newBody);

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
    bool isLeaf = false;
    body *theBody = new body(vec3(0, 0, 0), vec3(0, 0, 0), 0, 0, vec3(0, 0, 0), true); // Mass-less body by default
    bool calculatedCOM = false;

    // 3d array of the subdivisions of this octant
    bool divided = false;
    octant *subdivisions[2][2][2] = {nullptr}; // No sub-nodes by default

    void divide();

    octant *subdivisionEnclosing(body *b);

};


#endif //N_BODY_HEADLESS_OCTANT_H