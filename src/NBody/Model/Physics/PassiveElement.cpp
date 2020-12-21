//
// Created by jackcamp on 9/2/20.
//

#include "PassiveElement.h"

Model::Physics::PassiveElement::PassiveElement(float mass) :
        _position(nullptr),
        _velocity(nullptr),
        _mass(mass) {}

const Model::Position &Model::Physics::PassiveElement::position() const {
    return *_position;
}

Model::Velocity &Model::Physics::PassiveElement::velocity() {
    return *_velocity;
}
