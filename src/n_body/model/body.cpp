//
// Created by jackcamp on 2/25/19.
//

#include "body.h"

#include "../interface/viewport.h" // Viewport must be included in here to avoid cyclical dependencies

body::body(glm::vec3 position) {

    this->position = position;
    this->nextPosition = position;
}

body *body::setVelocity(glm::vec3 velocity) {

    this->nextVelocity = velocity;

    return this;
}

body *body::setMass(float mass) {

    this->mass = mass;

    calculateRadius();

    return this;
}

body *body::setDensity(float density) {

    this->density = density;

    calculateRadius();

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

void body::kick(glm::vec3 deltaV) {

    if (!fixed) {
        this->nextVelocity += deltaV;
    }

}

void body::drift(float deltaT) {

    this->nextPosition = this->position + (this->velocity * deltaT);
}

void body::shiftBuffers() {

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

void body::calculateRadius() {

    float volume = this->mass / this->density;

    this->radius = (float) pow((volume / float(M_PI)) * (3.0f / 4.0f), (1.0f / 3.0f));
}