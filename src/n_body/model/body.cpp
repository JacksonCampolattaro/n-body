//
// Created by jackcamp on 2/25/19.
//

#include "body.h"

body::body(glm::vec3 position) {

    this->position = position;
}

body *body::setVelocity(glm::vec3 velocity) {

    this->nextVelocity = velocity;

    return this;
}

body *body::setMass(float mass) {

    this->mass = mass;

    return this;
}

body *body::makeFixed() {

    this->fixed = true;

    return this;
}

body *body::makePassive() {

    this->passive = true;

    return this;
}

body *body::setColor(glm::vec3 color) {

    this->color = color;

    return this;
}

body *body::setRadius(float radius) {

    this->radius = radius;

    return this;
}

glm::vec3 body::getPosition() {

    return this->position;
}

glm::vec3 body::getNextPosition() {

    return this->nextPosition;
}

glm::vec3 body::getVelocity() {

    return this->velocity;
}

glm::vec3 body::getNextVelocity() {

    return this->nextVelocity;
}

float body::getMass() {

    return this->mass;
}

bool body::isFixed() {

    return this->fixed;
}

bool body::isPassive() {

    return this->passive;
}

void body::addVelocity(glm::vec3 deltaV) {

    if (!fixed) {
        this->velocity += deltaV;
    }

}

void body::update() {

    // Calculating the next position
    this->nextPosition = this->position + this->velocity;


    // Updating position and velocity
    this->position = this->nextPosition;
    this->velocity = this->nextVelocity;

}

void body::draw() {

    viewport::setColor(color);
    viewport::drawCircle(position, radius);

}

std::string body::toString() {

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
