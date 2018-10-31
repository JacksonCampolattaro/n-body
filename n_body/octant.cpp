//
// Created by jackcamp on 10/30/18.
//

#include "octant.h"

octant::octant(glm::vec3 location, float sideLength) {

    this->location = location;
}

void octant::addBody(body *theBody) {


    if (subdivisions[0] != nullptr) {
        // Selecting the location of the new body
        if (theBody->getPosition().x >= location.x) {
            // +
            if (theBody->getPosition().y >= location.y) {
                // ++
                if (theBody->getPosition().z >= location.z) {
                    // +++
                    subdivisions[1]->addBody(theBody);
                } else {
                    // ++-
                    subdivisions[2]->addBody(theBody);
                }
            } else {
                // +-
                if (theBody->getPosition().z >= location.z) {
                    // +-+
                    subdivisions[5]->addBody(theBody);
                } else {
                    // +--
                    subdivisions[6]->addBody(theBody);
                }
            }
        } else {
            // -
            if (theBody->getPosition().y >= location.y) {
                // -+
                if (theBody->getPosition().z >= location.z) {
                    // -++
                    subdivisions[4]->addBody(theBody);
                } else {
                    // -+-
                    subdivisions[3]->addBody(theBody);
                }
            } else {
                // --
                if (theBody->getPosition().z >= location.z) {
                    // --+
                    subdivisions[8]->addBody(theBody);
                } else {
                    // ---
                    subdivisions[7]->addBody(theBody);
                }
            }
        }
    }
}
