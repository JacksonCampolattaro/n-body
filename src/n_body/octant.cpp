//
// Created by jackcamp on 10/30/18.
//

#include <omp.h>
#include "octant.h"
#include "simulation.h"

octant::octant(glm::vec3 location, float sideLength) {

    this->location = location;
    this->sideLength = sideLength;
}

octant::~octant() = default;

void octant::addBody(vec3 newPosition, float newMass) {

    // Making room for new bodies by dividing if the node is a leaf
    if (isLeaf) {

        // Subdividing the octant
        divide();

        // Moving the body already contained
        subdivisionEnclosing(this->position)->addBody(this->position, this->mass);
    }

    // Adding the body to the appropriate subdivision if the node is divided
    if (divided) {

        // Adding the new body to the appropriate octant
        subdivisionEnclosing(newPosition)->addBody(newPosition, newMass);

        return;
    }

    // If the node doesnt yet contain any bodies at all
    this->position = newPosition;
    this->mass = newMass;
    isLeaf = true;
    calculatedCOM = true;
}

void octant::addBodies(std::vector<body*> newBodies) {

    // Preventing wasted time on an empty list
    if (newBodies.empty()) {
        return;
    }

    // Making room for new bodies by dividing if the node is a leaf (or if there is more than one body to add)
    if (isLeaf || newBodies.size() > 1) {
        divide();
    }

    // If the node is subdivided, all the bodies in the list will be added to the subdivisions
    if (divided) {


        // Array of lists to be added to each subdivision
        std::vector<body*> dividedBodies[2][2][2];


        // Checking which subdivision each body belongs to
        // TODO This can be made parallel if I switch to a thread safe data container
        for (body* theBody : newBodies) {

            // Comparing the new body's position to the center of the octant
            vec3 comparison = glm::greaterThanEqual(theBody->getPosition(), this->location);


            // Adding the body at the appropriate index
            dividedBodies[(int) comparison.x][(int) comparison.y][(int) comparison.z].push_back(theBody);
        }

        // Transferring the lists to the appropriate subdivisions
        #pragma omp parallel for collapse(3) // This should be thread safe because the subdivisions are independent
        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

                    // Adding the list of bodies to the appropriate subdivision
                    subdivisions[x][y][z]->addBodies(dividedBodies[x][y][z]);
                }
            }
        }

        // Making sure the current COM is marked false
        calculatedCOM = false;

        return;
    }

    // Base case, only one body left in the list, and the node isn't divided
    if (newBodies.size() == 1) {

        position = newBodies[0]->getPosition();
        mass = newBodies[0]->getMass();

        isLeaf = true;
        calculatedCOM = true;
    }
}

void octant::eightThreadAdd(std::vector<body *> newBodies) {

    // Preventing wasted time on an empty list
    if (newBodies.empty()) {
        return;
    }

    // Making room for new bodies by dividing if the node is a leaf (or if there is more than one body to add)
    if (isLeaf || newBodies.size() > 1) {
        divide();
    }

    // If the node is subdivided, all the bodies in the list will be added to the subdivisions
    if (divided) {


        // Array of lists to be added to each subdivision
        tbb::concurrent_queue<body*> dividedBodies[2][2][2];


        // Checking which subdivision each body belongs to
        #pragma omp parallel for
        for (int b = 0; b < newBodies.size(); ++b) {

            vec3 comparison = glm::greaterThanEqual(newBodies[b]->getPosition(), this->location);

            dividedBodies[(int) comparison.x][(int) comparison.y][(int) comparison.z].push(newBodies[b]);
        }

        /*for (body* theBody : newBodies) {

            // Comparing the new body's position to the center of the octant
            vec3 comparison = glm::greaterThanEqual(theBody->getPosition(), this->location);


            // Adding the body at the appropriate index
            dividedBodies[(int) comparison.x][(int) comparison.y][(int) comparison.z].push_back(theBody);
        }*/

        // Transferring the lists to the appropriate subdivisions
        #pragma omp parallel for collapse(3) // This should be thread safe because the subdivisions are independent
        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

                    subdivisions[x][y][z]->singleThreadProcess(dividedBodies[x][y][z]);
                }
            }
        }

        // Making sure the current COM is marked false
        calculatedCOM = false;

        return;
    }

    // Base case, only one body left in the list, and the node isn't divided
    if (newBodies.size() == 1) {

        position = newBodies[0]->getPosition();
        mass = newBodies[0]->getMass();

        isLeaf = true;
        calculatedCOM = true;
    }
}

void octant::singleThreadProcess(tbb::concurrent_queue<body *> newBodies) {

    // Iterates through the queue
    while (!newBodies.empty()) {

        // Getting the body to be added
        body* nextBody;
        newBodies.try_pop(nextBody);

        // Adding the body to the appropriate subnode
        this->addBody(nextBody->getPosition(), nextBody->getMass());
    }
}

void octant::enqueueBody(vec3 newPosition, float newMass) {
    positionsToBeProcessed.push(newPosition);
    massesToBeProcessed.push(newMass);
}

void octant::processBodyQueue() {

    // Preventing wasted time on an empty queue
    if (positionsToBeProcessed.empty()) {
        return;
    }

    // Making room for new bodies by dividing if the node is a leaf (or if there is more than one body to add)
    if (isLeaf || positionsToBeProcessed.unsafe_size() > 1) {
        divide();
    }

    // If the node is subdivided, all the bodies in the list will be added to the subdivisions
    if (divided) {

        // Getting the size of the queue to be processed
        unsigned long queueLength = positionsToBeProcessed.unsafe_size();

        // Checking which subdivision each body belongs to
        //#pragma parallel for
        for (int j = 0; j < queueLength; ++j) {

            // Retrieving the location and mass of the next body to be processed
            vec3 newPosition;
            positionsToBeProcessed.try_pop(newPosition);
            float newMass;
            massesToBeProcessed.try_pop(newMass);

            // Comparing the new body's position to the center of the octant to determine where it should go
            vec3 comparison = glm::greaterThanEqual(newPosition, this->location);

            // Adding the next body to the appropriate subdivision
            subdivisions[(int) comparison.x][(int) comparison.y][(int) comparison.z]->enqueueBody(newPosition, newMass);
        }

        // Repeating this entire process for each subdivision
        //#pragma omp parallel for collapse(3) // This should be thread safe because the subdivisions are independent
        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

                    // Each subdivision will distribute it's own queue in a new thread
                    subdivisions[x][y][z]->processBodyQueue();
                }
            }
        }

        // Making sure the current COM is marked false
        calculatedCOM = false;

        return;
    }

    // Base case, only one body left in the list, and the node isn't divided
    if (positionsToBeProcessed.unsafe_size() == 1) {

        // Attempts to set the position and mass
        positionsToBeProcessed.try_pop(position);
        massesToBeProcessed.try_pop(mass);

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
        #pragma omp parallel for collapse(3)
        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

                    // Ignoring nodes that don't contain any bodies
                    if (subdivisions[x][y][z]->isLeaf || subdivisions[x][y][z]->divided) {

                        // Recursively finding center of mass
                        subdivisions[x][y][z]->calculateCenterMass();

                        // Center of Mass = sum of masses * positions / total mass
                        COM += subdivisions[x][y][z].get()->getMass() *
                               subdivisions[x][y][z].get()->getPosition();

                        totalMass += subdivisions[x][y][z]->getMass();
                    }
                }
            }
        }

        COM = COM / totalMass;

        // Setting center of mass data
        this->position = COM;
        this->mass = totalMass;

        calculatedCOM = true;
        return;
    }


}

void octant::applyGravity(body *inputBody, float theta, simulation *simulation) {

    // Base case
    if (isLeaf) {

        // The input body is unaffected by any body with the same position, including itself
        if (position != inputBody->getPosition()) {
            simulation->applyGravity(inputBody, position, mass);
        }

    } else if (divided) {

        // Determining whether subdividing is necessary
        /* Node is treated as a single body if S/D < theta where S = sideLength and D = distance */
        if (theta > (float) sideLength / (float) glm::distance(inputBody->getPosition(), position)) {

            // Handling an un-calculated center of mass
            if (!calculatedCOM) {
                this->calculateCenterMass();
            }

            simulation->applyGravity(inputBody, position, mass);
        }
        /* Otherwise the node is subdivided */
        else {

            #pragma omp parallel for collapse(3)
            for (int x = 0; x <= 1; ++x) {
                for (int y = 0; y <= 1; ++y) {
                    for (int z = 0; z <= 1; ++z) {

                        // Recursively dividing the work
                        subdivisions[x][y][z]->applyGravity(inputBody, theta, simulation);
                    }
                }
            }
        }
    }
}

octant *octant::getSubdivision(ivec3 position) {

    // Gets the octant at this position
    // TODO I need to add vetting: positions must be either 0 or 1
    return subdivisions[position.x][position.y][position.z].get();
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
        theString +=
                indent + "┃ [Leaf Node] Position = (" + std::to_string(position.x) + ", " + std::to_string(position.y) +
                ", " + std::to_string(position.z) + ") Total Mass = " + std::to_string(mass) + "\n";
    } else {

        theString += indent + "┃ [Level " + std::to_string(level) + " Subnode]\n";
        theString += indent + "┃ Enclosing " + std::to_string(this->numBodies()) + " bodies" + "\n";
        theString += indent + "┃ Position = (" + std::to_string(location.x) + ", " + std::to_string(location.y) + ", " +
                     std::to_string(location.z) + ") Side Length = " + std::to_string(sideLength) + "\n";
        theString += indent + "┃ Center of Mass = (" + std::to_string(position.x) + ", " +
                     std::to_string(position.y) + ", " + std::to_string(position.z) +
                     ") Total Mass = " + std::to_string(mass) + "\n";

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

        float subdivisionSideLength = sideLength / 2;

        // Initializing all the subdivisions with their locations and sizes
        for (int x = 0; x <= 1; ++x) {
            for (int y = 0; y <= 1; ++y) {
                for (int z = 0; z <= 1; ++z) {

                    // Getting the sign in the x, y, and z axes
                    int xSign = (2 * x) - 1;
                    int ySign = (2 * y) - 1;
                    int zSign = (2 * z) - 1;

                    std::shared_ptr<octant> newNode(new octant(this->location +
                                                               glm::vec3(xSign * subdivisionSideLength,
                                                                         ySign * subdivisionSideLength,
                                                                         zSign * subdivisionSideLength),
                                                               subdivisionSideLength));

                    subdivisions[x][y][z] = newNode;
                }
            }
        }

        // Setting divided to true
        divided = true;
        isLeaf = false;
        calculatedCOM = false;

        return;
    }

}

std::shared_ptr<octant> octant::subdivisionEnclosing(vec3 position) {

    // Making sure the subdivisions exist
    if (!divided) {
        divide();
    }

    // Comparing the new body's position to the center of the octant
    vec3 comparison = glm::greaterThanEqual(position, this->location);

    // Adding the body at the appropriate index
    return subdivisions[(int) comparison.x][(int) comparison.y][(int) comparison.z];
}

const vec3 &octant::getPosition() const {
    return position;
}

float octant::getMass() const {
    return mass;
}




