//
// Created by jackcamp on 10/25/18.
//

#include "relationship.h"

relationship::relationship(body *firstBody, body *secondBody) {

    this->firstBody = firstBody;
    this->secondBody = secondBody;
}

void relationship::applyGravity(float interval, float gravitationalConstant, int power) {

    // Calculating the directionless force of gravity
    float forceOfGravity =
            (gravitationalConstant * firstBody->getMass() * secondBody->getMass()) /
            ((float) pow(distance(), power) + 0.0001f);

    // Giving the force direction
    //glm::vec3 force = forceOfGravity * unitVector();
    glm::vec3 force = forceOfGravity * unitVector();

    // Getting acceleration from both bodies
    vec3 firstAcceleration = firstBody->getAcceleration(force, interval);
    vec3 secondAcceleration = secondBody->getAcceleration(-force, interval);

    // Getting magnitudes of both accelerations
    float firstMagnitude = glm::length(firstAcceleration);
    float secondMagnitude = glm::length(secondAcceleration);

    // If either acceleration is too large and time resolution is within bounds
    if ((firstMagnitude > maximumAllowableAcceleration ||
        secondMagnitude > maximumAllowableAcceleration) &&
        interval > timeResolutionLimit) {

        // Decompose the operation into shorter timescales
        this->applyGravity(interval / 2.0f, gravitationalConstant, power);
        this->applyGravity(interval / 2.0f, gravitationalConstant, power);

    } else {
        // Base case

        firstBody->applyAcceleration(firstAcceleration, interval);
        secondBody->applyAcceleration(secondAcceleration, interval);
    }
}

std::string relationship::toString() {

    std::string theString = "Relationship : Distance = " + std::to_string(distance()) + "\n";
    theString += "┣━ " + firstBody->toString();
    theString += "┗━ " + secondBody->toString();

    return theString;
}

float relationship::distance() {
    return glm::distance(firstBody->getPosition(), secondBody->getPosition());
}

vec3 relationship::unitVector() {
    return glm::normalize(secondBody->getPosition() - firstBody->getPosition());
}
