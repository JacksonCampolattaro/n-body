//
// Created by jackcamp on 2/24/19.
//

#include "threadSafe_octant.h"


// Public methods

threadSafe_octant::threadSafe_octant(glm::vec3 location, float sideLength) {

    this->octantLocation = location;
    this->sideLength = sideLength;
}

void threadSafe_octant::addBody(glm::vec3 newPosition, float newMass) {

}

void threadSafe_octant::applyGravityToBody(body *theBody, simulation *theSimulation) {

    // Ignores empty nodes
    if (!initialized) {
        return;
    }

    // Base case 1: Leaf node
    if (!divided) {

        // Ignores the node if it contains the body to be acted upon
        if (centerOfMass != theBody->getPosition()) {

            theSimulation->applyGravity(theBody, centerOfMass, mass);
        }

        return;
    }

    // Base case 2: Divided, but subdivision is unnecessary
    /* Node is treated as a single body if S/D < theta (where S = sideLength and D = distance) */
    if (theSimulation->getTheta() > (float) sideLength / (float) distance(theBody->getPosition(), centerOfMass)) {

        theSimulation->applyGravity(theBody, centerOfMass, mass);

        return;
    }

    // Recursive case: Divided, and not far enough to avoid subdividing
    #pragma omp parallel for collapse(3)
    for (int x = 0; x <= 1; ++x) {
        for (int y = 0; y <= 1; ++y) {
            for (int z = 0; z <= 1; ++z) {

                // Recursively dividing the work
                children[x][y][z]->applyGravityToBody(theBody, theSimulation);
            }
        }
    }
}

std::string threadSafe_octant::toString(int level) {

    std::string theString;
    std::string indent;

    for (int i = 0; i < level; ++i) {
        indent += "┃ ";
    }

    theString += indent + "┏━━━ \n";

    if (!divided) {
        theString +=
                indent + "┃ [Leaf Node] Position = (" + std::to_string(centerOfMass.x) + ", " + std::to_string(centerOfMass.y) +
                ", " + std::to_string(centerOfMass.z) + ") Total Mass = " + std::to_string(mass) + "\n";
    } else {

        theString += indent + "┃ [Level " + std::to_string(level) + " Subnode]\n";
        theString += indent + "┃ Enclosing " + std::to_string(numBodies) + " bodies" + "\n";
        theString +=
                indent + "┃ Position = (" + std::to_string(octantLocation.x) + ", " + std::to_string(octantLocation.y) +
                ", " +
                std::to_string(octantLocation.z) + ") Side Length = " + std::to_string(sideLength) + "\n";
        theString += indent + "┃ Center of Mass = (" + std::to_string(centerOfMass.x) + ", " +
                     std::to_string(centerOfMass.y) + ", " + std::to_string(centerOfMass.z) +
                     ") Total Mass = " + std::to_string(mass) + "\n";

        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

                    if (children[x][y][z]->initialized) {

                        theString += children[x][y][z]->toString(level + 1);

                    }
                }
            }
        }
    }

    theString += indent + "┗━━━ \n";

    return theString;
}

int threadSafe_octant::getNumBodies() {
    return numBodies;
}

vec3 threadSafe_octant::getCenterOfMass() {

    // If the center of mass hasn't been calculated, calculate it
    if (!validCenterOfMass) {
        vec3 COM = vec3(0, 0, 0);

        // Summing all subdivisions, safely done in parallel
        #pragma omp parallel for collapse(3)
        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

                    // Nodes that don't contain any bodies are ignored
                    if (children[x][y][z]->initialized) {

                        // Adding each position, weighted by mass
                        /*Center of Mass = (sum of masses * positions) / total mass*/
                        COM += children[x][y][z]->mass *
                                children[x][y][z]->getCenterOfMass();
                    }
                }
            }
        }

        // Dividing by the total mass
        /*Center of Mass = (sum of masses * positions) / total mass*/
        centerOfMass = COM / mass;

    }

    // Returns the position held by the octant
    /*Note: Position is a null vector if the octant is not initialized*/
    return centerOfMass;
}

vec3 threadSafe_octant::getAveragePosition() {

    // If the average position hasn't been calculated, calculate it
    if (!validAveragePosition) {
        vec3 avg = vec3(0, 0, 0);

        // Summing all subdivisions, safely done in parallel
        #pragma omp parallel for collapse(3)
        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

                    // Nodes that don't contain any bodies are ignored
                    if (children[x][y][z]->initialized) {

                        // Adding each position, unweighted
                        /*Average Position = sum of positions / number of positions*/
                        avg += children[x][y][z]->getAveragePosition();
                    }
                }
            }
        }

        // Dividing by the total number of bodies in this tree
        /*Average Position = sum of positions / number of positions*/
        averagePosition = avg / (float) numBodies;

    }

    return averagePosition;
}


// Private methods

void threadSafe_octant::divide() {

    /*// This might be the more correct way to do this...
    static volatile std::atomic_bool divided;*/

    // If the subdivisions haven't been initialized yet
    /*Because divided is atomic, this can only ever happen once per node*/
    if (!divided) {

        // The child nodes are half the size of this one
        float subdivisionSideLength = sideLength / 2;

        // Initializing all the subdivisions with their locations and sizes
        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

                    // Getting the sign in the x, y, and z axes
                    int xSign = (2 * x) - 1;
                    int ySign = (2 * y) - 1;
                    int zSign = (2 * z) - 1;

                    // Creating the node with the right position
                    std::shared_ptr<threadSafe_octant> newNode(new threadSafe_octant(this->octantLocation +
                                                               glm::vec3(xSign * subdivisionSideLength,
                                                                         ySign * subdivisionSideLength,
                                                                         zSign * subdivisionSideLength),
                                                               subdivisionSideLength));

                    // Adding the node to the right index
                    children[x][y][z] = newNode;
                }
            }
        }

        // Setting divided to true
        divided = true;

        // Invalidates the center of mass, so it should be recalculated before it is used
        validCenterOfMass = false;

        return;
    }

}

shared_ptr<threadSafe_octant> threadSafe_octant::getSubdivisionEnclosing(vec3 position) {

    // Making sure the subdivisions exist
    if (!divided) {
        divide();
    }

    // Comparing the new body's position to the center of the octant
    vec3 comparison = glm::greaterThanEqual(position, this->octantLocation);

    // Adding the body at the appropriate index
    return children[(int) comparison.x][(int) comparison.y][(int) comparison.z];
}


