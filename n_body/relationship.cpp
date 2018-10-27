//
// Created by jackcamp on 10/25/18.
//

#include "relationship.h"

relationship::relationship(orbitalBody *firstBody, orbitalBody *secondBody) {

    this->firstBody = firstBody;
    this->secondBody = secondBody;
}

void relationship::applyGravity(float interval, float gravitationalConstant, int power) {

    // Calculating the directionless force of gravity
    float forceOfGravity =
            (gravitationalConstant * firstBody->getMass() * secondBody->getMass()) /
            ((float) pow(distance(), power) + 0.01f);

    // Giving the force direction
    //glm::vec3 force = forceOfGravity * unitVector();
    glm::vec3 force = forceOfGravity * unitVector();

    // Applying the force to both bodies
    firstBody->applyForce(force, interval);
    secondBody->applyForce(-force, interval);
}

float relationship::distance() {
    return glm::distance(firstBody->getPosition(), secondBody->getPosition());
}

vec3 relationship::unitVector() {
    return glm::normalize(secondBody->getPosition() - firstBody->getPosition());
}
