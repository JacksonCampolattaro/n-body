//
// Created by jackcamp on 2/25/19.
//

#include "Body.h"

Body::Body(Position position) : position(position), nextPosition(position) {}

Body *Body::setVelocity(Velocity velocity) {

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

Body *Body::setColor(rgbaColor color) {

    this->color = color;

    return this;
}

Body *Body::setRadius(float radius) {

    this->radius = radius;

    return this;
}

glm::vec3 Body::getPosition() const {

    return this->position;
}

glm::vec3 Body::getNextPosition() const {

    return this->nextPosition;
}

glm::vec3 Body::getVelocity() const {

    return this->velocity;
}

glm::vec3 Body::getNextVelocity() const {

    return this->nextVelocity;
}

float Body::getMass() const {

    return this->mass;
}

bool Body::isFixed() const {

    return this->fixed;
}

bool Body::isPassive() const {

    return this->passive;
}

void Body::kick(glm::vec3 deltaV) {

    if (!fixed) {
        this->nextVelocity += deltaV;
    }

}

void Body::drift(float deltaT) {

    this->nextPosition = Position(this->position + (this->velocity * deltaT));
}

void Body::shiftBuffers() {

    // Updating position and velocity
    /*Atomic write is used so that the renderer can access position values safely*/

#pragma omp atomic write
    position.x = nextPosition.x;
#pragma omp atomic write
    position.y = nextPosition.y;
#pragma omp atomic write
    position.z = nextPosition.z;

#pragma omp atomic write
    velocity.x = nextVelocity.x;
#pragma omp atomic write
    velocity.y = nextVelocity.y;
#pragma omp atomic write
    velocity.z = nextVelocity.z;

}

void Body::draw() {

    /*DrawSphereEx(Vector3{position.x, position.y, position.z},
                 radius,
                 10, 10,
                 Color{(unsigned char) (color.r * 255),
                       (unsigned char) (color.g * 255),
                       (unsigned char) (color.b * 255),
                       (unsigned char) (color.a * 255),});*/
}

std::string Body::toString() const {

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

    this->radius = std::pow((volume / float(M_PI)) * (3.0f / 4.0f), (1.0f / 3.0f));
}