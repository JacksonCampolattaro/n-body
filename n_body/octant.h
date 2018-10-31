//
// Created by jackcamp on 10/30/18.
//

#ifndef N_BODY_HEADLESS_OCTANT_H
#define N_BODY_HEADLESS_OCTANT_H

#include <glm/glm.hpp>
#include "body.h"


class octant {

public:

    octant(glm::vec3 location, float sideLength);

    void addBody(body *theBody);

private:

    // Location of the octant
    glm::vec3 location;

    // Bounds of the octant
    glm::vec3 bounds;

    // The body contained if this is the lowest node
    body *theBody = nullptr; // No body by default

    // 3d array of the subdivisions of this octant
    octant *subdivisions[8] = {nullptr}; // No subnodes by default

};


#endif //N_BODY_HEADLESS_OCTANT_H
