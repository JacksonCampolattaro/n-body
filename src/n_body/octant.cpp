//
// Created by jackcamp on 10/30/18.
//

#include "octant.h"

octant::octant(glm::vec3 location, float sideLength) {

    this->location = location;
    this->sideLength = sideLength;
}

void octant::addBody(body *newBody) {

    // If this octant has already been divided into subdivisions
    if (divided) {
        subdivisionEnclosing(theBody)->addBody(theBody);
        return;
    }

    // If a body has already been added
    if (occupied) {

        // Initializing the subdivisions
        divide();

        // Adding the body at the appropriate index
        subdivisionEnclosing(newBody)->addBody(newBody);

        // Moving the body already contained
        subdivisionEnclosing(theBody)->addBody(theBody);
        theBody = nullptr;

        return;

    }

    this->theBody = newBody;
    occupied = true;
}

void octant::divide() {

    // If the subdivisions haven't been initialized yet
    if (!divided) {

        float offset = sideLength / 4;

        // Initializing all the subdivisions with their locations and sizes
        for (int x = 0; x < 1; ++x) {
            for (int y = 0; y < 1; ++y) {
                for (int z = 0; z < 1; ++z) {

                    // Getting the sign in the x, y, and z axes
                    int xSign = (2 * x) - 1;
                    int ySign = (2 * y) - 1;
                    int zSign = (2 * z) - 1;

                    // Initializing the subdivision
                    subdivisions[x][y][z] = new octant(
                            location += glm::vec3(xSign * offset, ySign * offset, zSign * offset), sideLength / 2);
                }
            }
        }

        // Setting divided to true
        divided = true;
    }

}

octant *octant::subdivisionEnclosing(body *b) {

    // Comparing the new body's position to the center of the octant
    vec3 comparison = glm::greaterThanEqual(b->getPosition(), location);

    // Adding the body at the appropriate index
    return subdivisions[(int) comparison.x][(int) comparison.y][(int) comparison.z];
}


