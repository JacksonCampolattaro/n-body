//
// Created by jackcamp on 10/30/18.
//

#include "octant.h"

octant::octant(glm::vec3 location, float sideLength) {

    this->location = location;
    this->sidelength = sideLength;
}

void octant::addBody(body *newBody) {


    // If a body has already been added
    if (occupied) {

        // If the subdivisions haven't been initialized yet
        if (subdivisions[0] == nullptr) {

            float offset = sidelength / 4;

            // Creating each of the subdivisions
            subdivisions[0] = new octant(location += glm::vec3(offset, offset, offset), sidelength / 2);
            subdivisions[1] = new octant(location += glm::vec3(offset, offset, -offset), sidelength / 2);
            subdivisions[2] = new octant(location += glm::vec3(-offset, offset, -offset), sidelength / 2);
            subdivisions[3] = new octant(location += glm::vec3(-offset, offset, offset), sidelength / 2);
            subdivisions[4] = new octant(location += glm::vec3(offset, -offset, offset), sidelength / 2);
            subdivisions[5] = new octant(location += glm::vec3(offset, -offset, -offset), sidelength / 2);
            subdivisions[6] = new octant(location += glm::vec3(-offset, -offset, -offset), sidelength / 2);
            subdivisions[7] = new octant(location += glm::vec3(-offset, -offset, offset), sidelength / 2);
        }

        // Selecting the location of the new body
        if (newBody->getPosition().x >= location.x) {
            // +
            if (newBody->getPosition().y >= location.y) {
                // ++
                if (newBody->getPosition().z >= location.z) {
                    // +++
                    subdivisions[0]->addBody(theBody);
                    subdivisions[0]->addBody(newBody);
                } else {
                    // ++-
                    subdivisions[1]->addBody(theBody);
                    subdivisions[1]->addBody(newBody);
                }
            } else {
                // +-
                if (newBody->getPosition().z >= location.z) {
                    // +-+
                    subdivisions[4]->addBody(theBody);
                    subdivisions[4]->addBody(newBody);
                } else {
                    // +--
                    subdivisions[5]->addBody(theBody);
                    subdivisions[5]->addBody(newBody);
                }
            }
        } else {
            // -
            if (newBody->getPosition().y >= location.y) {
                // -+
                if (newBody->getPosition().z >= location.z) {
                    // -++
                    subdivisions[3]->addBody(theBody);
                    subdivisions[3]->addBody(newBody);
                } else {
                    // -+-
                    subdivisions[2]->addBody(theBody);
                    subdivisions[2]->addBody(newBody);
                }
            } else {
                // --
                if (newBody->getPosition().z >= location.z) {
                    // --+
                    subdivisions[7]->addBody(theBody);
                    subdivisions[7]->addBody(newBody);
                } else {
                    // ---
                    subdivisions[6]->addBody(theBody);
                    subdivisions[6]->addBody(newBody);
                }
            }
        }
    } else {
        // In cases where the first body has not been added yet
        this->theBody = newBody;

    }

    occupied = true;
}
