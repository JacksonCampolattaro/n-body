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

        this->body1 = firstBody;
        this->body2 = secondBody;
    }

    /**
     * Applies the gravity between the two bodies.
     */
    void applyGravity() {

        // todo apply the gravity between the two bodies
    }


    /**
     * Calculates the distance between the two bodies
     * @return the distance as a float
     */
    float distance() {
        return glm::distance(body1->getPosition(), body2->getPosition());
    }

private:

    body *body1;
    body *body2;

};


#endif //N_BODY_HEADLESS_RELATIONSHIP_H
