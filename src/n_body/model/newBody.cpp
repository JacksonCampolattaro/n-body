//
// Created by jackcamp on 6/18/19.
//

#include <cereal/cereal.hpp>
#include "newBody.h"


newBody::newBody() {

}

newBody *newBody::setPosition(Position position) {
    return nullptr;
}

newBody *newBody::setVelocity(Velocity velocity) {
    return nullptr;
}

newBody *newBody::setMass(float mass) {
    return nullptr;
}

newBody *newBody::makeFixed(bool fixed) {
    return nullptr;
}

newBody *newBody::makePassive(bool passive) {
    return nullptr;
}

newBody *newBody::setRenderer(Renderer renderer) {
    return nullptr;
}

Position newBody::getPosition() {
    return Position(0, 0, 0);
}

Velocity newBody::getVelocity() {
    return Velocity(0, 0, 0);
}

float newBody::getMass() {
    return 0;
}

bool newBody::isFixed() {
    return false;
}

bool newBody::isPassive() {
    return false;
}

void newBody::shiftBuffers() {

}

Renderer newBody::getRenderer() {
    return Renderer();
}

std::string newBody::toString() {
    return std::__cxx11::string();
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