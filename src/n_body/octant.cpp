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
    if (isLeaf) {

        // Initializing the subdivisions
        divide();

        // Moving the body already contained
        subdivisionEnclosing(theBody)->addBody(theBody);

        // Adding the body at the appropriate index
        subdivisionEnclosing(newBody)->addBody(newBody);

        isLeaf = false;
        theBody = nullptr;

        return;

    }

    this->theBody = newBody;
    isLeaf = true;
}

void octant::calculateCenterMass() {

    // First base case: contains only one body
    if (isLeaf) {
        return;
    }

    // Getting center of mass of non-leaf node
    if (divided) {

        glm::vec3 COM = glm::vec3(0, 0, 0);
        float totalMass = 0;

        // Iterating through all subdivisions
        for (int x = 0; x < 1; ++x) {
            for (int y = 0; y < 1; ++y) {
                for (int z = 0; z < 1; ++z) {

                    // Recursively finding center of mass
                    subdivisions[x][y][z]->calculateCenterMass();

                    // Center of Mass = sum of masses * positions / total mass
                    COM += subdivisions[x][y][z]->getCenterMass()->getMass() *
                           subdivisions[x][y][z]->getCenterMass()->getPosition();

                    totalMass += subdivisions[x][y][z]->getCenterMass()->getMass();
                }
            }
        }

        COM = COM / totalMass;

        // A placeholder body is created representing the center of mass
        theBody = new body(COM, vec3(0, 0, 0), totalMass, 0, vec3(0, 0, 0), true);
    }

    // Second base case: contains no bodies
    theBody = new body(vec3(0, 0, 0), vec3(0, 0, 0), 0, 0, vec3(0, 0, 0), true);
}

body *octant::getCenterMass() {

    return theBody;
}

octant *octant::getSubdivision(ivec3 position) {

    // Gets the octant at this position
    // TODO I need to add vetting: positions must be either 0 or 1
    return subdivisions[position.x][position.y][position.z];
}

void octant::divide() {

    // If the subdivisions haven't been initialized yet
    if (!divided) {

        float offset = sideLength / 4;

        // Initializing all the subdivisions with their locations and sizes
        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

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


