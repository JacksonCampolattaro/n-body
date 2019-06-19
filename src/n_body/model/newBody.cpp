//
// Created by jackcamp on 6/18/19.
//

#include <cereal/cereal.hpp>
#include "newBody.h"


newBody::newBody() {

}

newBody *newBody::setPosition(Position position) {
    this->position = position;
    return this;
}

newBody *newBody::setVelocity(Velocity velocity) {
    this->velocity = velocity;
    return this;
}

newBody *newBody::setMass(float mass) {
    this->mass = mass;
    return this;
}

newBody *newBody::makeFixed(bool fixed) {
    this->fixed = fixed;
    return this;
}

newBody *newBody::makePassive(bool passive) {
    this->passive = passive;
    return this;
}

newBody *newBody::setRenderer(Renderer renderer) {
    this->renderer = renderer;
    return this;
}

Position newBody::getPosition() {
    return this->position;
}

Velocity newBody::getVelocity() {
    return this->velocity;
}

float newBody::getMass() {
    return mass;
}

bool newBody::isFixed() {
    return fixed;
}

bool newBody::isPassive() {
    return passive;
}

void newBody::shiftBuffers() {

    // Updating position and velocity
    this->position = this->nextPosition;
    this->velocity = this->nextVelocity;
}

Renderer newBody::getRenderer() {
    // TODO the renderer's position value will be updated before returning it
    return renderer;
}

std::string newBody::toString() {

    std::string s =
            "Body : Position = " + position.toString() +
            " Velocity = " + velocity.toString() +
            " Mass = " + std::to_string(mass);

    if (fixed) {
        s += " [Fixed]";
    }

    if (passive) {
        s += " [Passive]";
    }

    s += "\n";

    return s;
}

template<class Archive>
void newBody::serialize(Archive &ar) {

    // Most variables are added to the archive, but not the buffers
    ar(
            CEREAL_NVP(position), CEREAL_NVP(velocity), /* The position and velocity serialize themselves */
            CEREAL_NVP(mass), CEREAL_NVP(fixed), CEREAL_NVP(passive),
            CEREAL_NVP(renderer) /* The renderer handles its own serialization, too */
    );

    // The buffers are flushed and now match initial conditions
    this->nextPosition = this->position;
    this->nextVelocity = this->velocity;
}