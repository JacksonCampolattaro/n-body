//
// Created by jackcamp on 9/2/20.
//

#include "PassiveElement.h"

Model::Physics::PassiveElement::PassiveElement(Model::Handle<Model::Position> position,
                                               Model::Handle<Model::Velocity> velocity, float mass) :
                                               _position(position),
                                               _velocity(velocity),
                                               _mass(mass)
                                               {}

const Model::Position &Model::Physics::PassiveElement::position() const {
    return *_position;
}

Model::Velocity &Model::Physics::PassiveElement::velocity() {
    return *_velocity;
}
