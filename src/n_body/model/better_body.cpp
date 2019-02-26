//
// Created by jackcamp on 2/25/19.
//

#include "better_body.h"

better_body::better_body(glm::vec3 position) {

    this->position = position;
}

better_body *better_body::setVelocity(glm::vec3 velocity) {

    this->nextVelocity = velocity;

    return this;
}

better_body *better_body::setMass(float mass) {

    this->mass = mass;

    return this;
}

better_body *better_body::makeFixed() {

    this->fixed = true;

    return this;
}

better_body *better_body::makePassive() {

    this->passive = true;

    return this;
}

better_body *better_body::setColor(glm::vec3 color) {

    this->color = color;

    return this;
}

better_body *better_body::setRadius(float radius) {

    this->radius = radius;

    return this;
}

glm::vec3 better_body::getPosition() {

    return this->position;
}

glm::vec3 better_body::getNextPosition() {

    return this->nextPosition;
}

glm::vec3 better_body::getVelocity() {

    return this->velocity;
}

glm::vec3 better_body::getNextVelocity() {

    return this->nextVelocity;
}

float better_body::getMass() {

    return this->mass;
}

bool better_body::isFixed() {

    return this->fixed;
}

bool better_body::isPassive() {

    return this->passive;
}

void better_body::addVelocity(glm::vec3 deltaV) {

    if (!fixed) {
        this->velocity += deltaV;
    }

}

void better_body::update() {

    // Calculating the next position
    this->nextPosition = this->position + this->velocity;


    // Updating position and velocity
    this->position = this->nextPosition;
    this->velocity = this->nextVelocity;

}

void better_body::draw() {

    viewport::setColor(color);
    viewport::drawCircle(position, radius);

}

std::string better_body::toString() {

    std::string
            theString = "Body : Position = (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " +
                        std::to_string(position.z) + ") Velocity = <" + std::to_string(velocity.x) + ", " +
                        std::to_string(velocity.y) + ", " + std::to_string(velocity.z) + "> Mass = " +
                        std::to_string(mass);

    if (fixed) {
        theString += " [Fixed]";
    }

    if (passive) {
        theString += " [Passive]";
    }

    theString += "\n";

    return theString;
}
