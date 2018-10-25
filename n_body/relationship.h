//
// Created by jackcamp on 10/25/18.
//

#ifndef N_BODY_HEADLESS_RELATIONSHIP_H
#define N_BODY_HEADLESS_RELATIONSHIP_H

#include <glm/glm.hpp>
#include "orbitalBody.h"

class relationship {

public:

    /**
     * Creates a new relationship between two bodies
     *
     * @param firstBody one body
     * @param secondBody another body
     */
    relationship(orbitalBody *firstBody, orbitalBody *secondBody) {

        this->firstBody = firstBody;
        this->secondBody = secondBody;
    }

    /**
     * Applies the gravity between the two bodies according to the standard equation.
     *
     * @param interval the time interval over which gravity should be applied
     * @param gravitationalConstant the constant of gravity 'G'
     * @param power the power of the distance factor (e.g Gmm/d^2 vs Gmm/d^1)
     */
    void applyGravity(float interval, float gravitationalConstant = 0.03, int power = 2) {

        // Calculating the directionless force of gravity
        float forceOfGravity =
                (gravitationalConstant * firstBody->getMass() * secondBody->getMass()) /
                ((float) pow(distance(), power));

        // Giving the force direction
        //glm::vec3 force = forceOfGravity * unitVector();
        glm::vec3 force = forceOfGravity * glm::vec3(1, 0, 0);

        // Applying the force to both bodies
        firstBody->applyForce(force, interval);
        secondBody->applyForce(-force, interval);
    }

private:

    /**
     * Calculates the distance between the two bodies
     * @return the distance as a float
     */
    float distance() {
        return glm::distance(firstBody->getPosition(), secondBody->getPosition());
    }

    /**
     * Creates a unit vector which represents the relationship between the bodies
     *
     * @return unit vector of the line between the bodies
     */
    vec3 unitVector() {
        return glm::normalize(secondBody->getPosition() - firstBody->getPosition());
    }

    orbitalBody *firstBody;
    orbitalBody *secondBody;

};


#endif //N_BODY_HEADLESS_RELATIONSHIP_H
