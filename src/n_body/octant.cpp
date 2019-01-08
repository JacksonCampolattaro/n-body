//
// Created by jackcamp on 10/30/18.
//

#include "octant.h"
#include <exception>

octant::octant(glm::vec3 location, float sideLength) {

    this->location = location;
    this->sideLength = sideLength;
}

void octant::addBody(body *newBody) {

    // If this octant has already been divided into subdivisions
    if (divided) {
        subdivisionEnclosing(newBody)->addBody(newBody);
        calculatedCOM = false;
        return;
    }

        // If a body has already been added
    else if (isLeaf) {

        // Initializing the subdivisions
        divide();

        // Moving the body already contained
        subdivisionEnclosing(theBody)->addBody(theBody);

        // Adding the body at the appropriate index
        subdivisionEnclosing(newBody)->addBody(newBody);

        isLeaf = false;
        divided = true;
        calculatedCOM = false;
        return;
    }

        // If the node doesnt yet contain any bodies at all
    else {

        this->theBody = newBody;
        isLeaf = true;
        calculatedCOM = true;
    }
}

void octant::calculateCenterMass() {

    if (calculatedCOM) {
        return;
    }

    // Base case: contains only one body
    if (isLeaf) {

        // The center of mass is the body contained
        calculatedCOM = true;
        return;
    }

    // Getting center of mass of non-leaf node
    if (divided) {

        // The center of mass will be calculated from all subnodes
        glm::vec3 COM = glm::vec3(0, 0, 0);
        float totalMass = 0;

        // Iterating through all subdivisions
        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

                    // Ignoring nodes that don't contain any bodies
                    if (subdivisions[x][y][z]->isLeaf || subdivisions[x][y][z]->divided) {

                        // Recursively finding center of mass
                        subdivisions[x][y][z]->calculateCenterMass();

                        // Center of Mass = sum of masses * positions / total mass
                        COM += subdivisions[x][y][z]->getCenterMass()->getMass() *
                               subdivisions[x][y][z]->getCenterMass()->getPosition();

                        totalMass += subdivisions[x][y][z]->getCenterMass()->getMass();
                    }
                }
            }
        }

        COM = COM / totalMass;

        // A placeholder body is created representing the center of mass
        theBody = new body(COM, vec3(0, 0, 0), totalMass, 0, vec3(0, 0, 0), true);

        calculatedCOM = true;
        return;
    }


}

body *octant::getCenterMass() {

    // Handling un-calculated center of mass
    if (!calculatedCOM) {
        this->calculateCenterMass();
    }

    return theBody;
}

std::vector<relationship *> octant::getRelationships(body *inputBody, float theta) {

    std::vector<relationship *> relationships;

    // Base case
    if (isLeaf) {

        // The input body is unaffected by any body with the same position, including itself
        if (theBody->getPosition() == inputBody->getPosition()) {
            return relationships;
        }

        // Otherwise the relationship is added to the list
        relationships.push_back(new relationship(inputBody, theBody));;
    }

        // Recursive case
    else if (divided) {

        // Handling an un-calculated center of mass
        if (!calculatedCOM) {
            this->calculateCenterMass();
        }

        // Determining whether subdividing is necessary
        /* Node is treated as a single body if S/D > theta where S = sideLength and D = distance*/
        if (theta > (float) sideLength / (float) glm::distance(inputBody->getPosition(), theBody->getPosition())) {

            relationships.push_back(new relationship(inputBody, theBody));
        }
            /* Otherwise the node is subdivided */
        else {

            for (int x = 0; x <= 1; ++x) {
                for (int y = 0; y <= 1; ++y) {
                    for (int z = 0; z <= 1; ++z) {

                        // Getting all the relationships from this subdivision
                        std::vector<relationship *> recursiveList = subdivisions[x][y][z]->getRelationships(inputBody,
                                                                                                            theta);

                        relationships.insert(std::end(relationships), std::begin(recursiveList),
                                             std::end(recursiveList));
                    }
                }
            }
        }
    }

    return relationships;
}

octant *octant::getSubdivision(ivec3 position) {

    // Gets the octant at this position
    // TODO I need to add vetting: positions must be either 0 or 1
    return subdivisions[position.x][position.y][position.z];
}

int octant::numBodies() {

    // Base case
    if (isLeaf) {
        return 1;
    }

    int total = 0;

    if (divided) {

        // Recursively checking lower subnodes
        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

                    total += subdivisions[x][y][z]->numBodies();
                }
            }
        }

    }

    return total;
}

std::string octant::toString(int level) {

    std::string theString = "";
    std::string indent = "";

    for (int i = 0; i < level; ++i) {
        indent += "┃ ";
    }

    theString += indent + "┏━━━ \n";

    if (isLeaf) {
        theString += indent + "┃ [Leaf Node] " + theBody->toString();
    } else {

        theString += indent + "┃ [Level " + std::to_string(level) + " Subnode]\n";
        theString += indent + "┃ Enclosing " + std::to_string(this->numBodies()) + " bodies" + "\n";
        theString += indent + "┃ Position = (" + std::to_string(location.x) + ", " + std::to_string(location.y) + ", " +
                std::to_string(location.z) + ") Side Length = " + std::to_string(sideLength) + "\n";
        theString += indent + "┃ Center of Mass = (" + std::to_string(theBody->getPosition().x) + ", " +
                     std::to_string(theBody->getPosition().y) + ", " + std::to_string(theBody->getPosition().z) +
                     ") Total Mass = " + std::to_string(theBody->getMass()) + "\n";

        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

                    if (subdivisions[x][y][z]->isLeaf || subdivisions[x][y][z]->divided) {

                        theString += subdivisions[x][y][z]->toString(level + 1);

                    }
                }
            }
        }
    }

    theString += indent + "┗━━━ \n";

    return theString;
}

void octant::divide() {

    // If the subdivisions haven't been initialized yet
    if (!divided) {

        float offset = sideLength / 2;
        float subdivisionSideLength = sideLength / 2;

        // Initializing all the subdivisions with their locations and sizes
        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

                    // Getting the sign in the x, y, and z axes
                    int xSign = (2 * x) - 1;
                    int ySign = (2 * y) - 1;
                    int zSign = (2 * z) - 1;

                    // Initializing the subdivision
                    octant *newNode = new octant(
                            this->location + glm::vec3(xSign * subdivisionSideLength,
                                                       ySign * subdivisionSideLength,
                                                       zSign * subdivisionSideLength), subdivisionSideLength);

                    subdivisions[x][y][z] = newNode;
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


