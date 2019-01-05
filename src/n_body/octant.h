//
// Created by jackcamp on 10/30/18.
//

#ifndef N_BODY_HEADLESS_OCTANT_H
#define N_BODY_HEADLESS_OCTANT_H

#include <glm/glm.hpp>
#include "body.h"


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
    body* getCenterMass();

    /**
     * Returns the requested subdivision of the octant
     * @return one of the eight octants contained by the parent
     */
    octant* getSubdivision(ivec3);

private:

    // Location of the octant
    glm::vec3 location;

    // Bounds of the octant
    float sideLength;

    // The body contained if this is the lowest node
    bool isLeaf = false;
    body *theBody = nullptr; // No body by default

    // 3d array of the subdivisions of this octant
    bool divided = false;
    octant *subdivisions[2][2][2] = {nullptr}; // No sub-nodes by default

    void divide();

    octant *subdivisionEnclosing(body *b);

};


#endif //N_BODY_HEADLESS_OCTANT_H