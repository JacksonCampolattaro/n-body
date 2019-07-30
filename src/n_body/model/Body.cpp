//
// Created by jackcamp on 2/25/19.
//

#include "Body.h"
#include <raylib.h>

Body::Body(glm::vec3 position) {

    this->position = position;
    this->nextPosition = position;
}

Body *Body::setVelocity(glm::vec3 velocity) {

    this->velocity = velocity;
    this->nextVelocity = velocity;

    return this;
}

Body *Body::setMass(float mass) {

    this->mass = mass;

    calculateRadius();

    return this;
}

Body *Body::setDensity(float density) {

    this->density = density;

    calculateRadius();

    return this;
}

Body *Body::makeFixed() {

    this->fixed = true;

    return this;
}

Body *Body::makePassive() {

    this->passive = true;

    return this;
}

Body *Body::setColor(glm::vec3 color) {

    this->color = color;

    return this;
}

Body *Body::setRadius(float radius) {

    this->radius = radius;

    return this;
}

glm::vec3 Body::getPosition() {

    return this->position;
}

glm::vec3 Body::getNextPosition() {

    return this->nextPosition;
}

glm::vec3 Body::getVelocity() {

    return this->velocity;
}

glm::vec3 Body::getNextVelocity() {

    return this->nextVelocity;
}

float Body::getMass() {

    return this->mass;
}

bool Body::isFixed() {

    return this->fixed;
}

bool Body::isPassive() {

    return this->passive;
}

void Body::kick(glm::vec3 deltaV) {

    if (!fixed) {
        this->nextVelocity += deltaV;
    }

}

void Body::drift(float deltaT) {

    this->nextPosition = this->position + (this->velocity * deltaT);
}

void Body::shiftBuffers() {

    // Updating position and velocity
    this->position = this->nextPosition;
    this->velocity = this->nextVelocity;

}

void Body::draw() {

    DrawSphereEx(Vector3{position.x, position.y, position.z}, radius, 10, 10, BLUE);
}

std::string Body::toString() {

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

void Body::calculateRadius() {

    float volume = this->mass / this->density;

    this->radius = (float) pow((volume / float(M_PI)) * (3.0f / 4.0f), (1.0f / 3.0f));
}