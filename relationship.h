//
// Created by jackcamp on 10/18/18.
//

#ifndef N_BODY_HEADLESS_RELATIONSHIP_H
#define N_BODY_HEADLESS_RELATIONSHIP_H


#include "body.h"

/**
 * Contains a pair of bodies and methods for the relationship between them.
 */
class relationship {

public:

    /**
     * Creates a new relationship between two bodies
     *
     * @param firstBody one body
     * @param secondBody another body
     */
    relationship(body *firstBody, body *secondBody) {

        this->firstBody = firstBody;
        this->secondBody = secondBody;
    }

    /**
     * Applies the gravity between the two bodies.
     *
     * Gravitational constant is set by default but can be overridden
     */
    void applyGravity(float gravitationalConstant = 0.03) {

        // todo apply the gravity between the two bodies

        float forceOfGravity =
                (gravitationalConstant * firstBody->getMass() * secondBody->getMass()) / distance();


    }


    /**
     * Calculates the distance between the two bodies
     * @return the distance as a float
     */
    float distance() {
        return glm::distance(firstBody->getPosition(), secondBody->getPosition());
    }

private:

    /**
     * Creates a unit vector which represents the relationship between the bodies
     *
     * @return unit vector of the line between the bodies
     */
    vec3 unitVector() {
        return normalize(secondBody->getPosition() - firstBody->getPosition());
    }

    body *firstBody;
    body *secondBody;

};


#endif //N_BODY_HEADLESS_RELATIONSHIP_H
