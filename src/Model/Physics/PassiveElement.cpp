//
// Created by jackcamp on 9/2/20.
//

#include "PassiveElement.h"

Model::Physics::PassiveElement::PassiveElement(float mass) :
        _mass(mass),
        _positionVector(nullptr),
        _positionIndex(0),
        _velocityVector(nullptr),
        _velocityIndex(0) {

}

const Model::Position &Model::Physics::PassiveElement::position() const {
    return (*_positionVector)[_positionIndex];
}

Model::Velocity &Model::Physics::PassiveElement::velocity() {
    return (*_velocityVector)[_velocityIndex];
}
