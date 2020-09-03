//
// Created by jackcamp on 9/2/20.
//

#include "ActiveElement.h"

Model::Physics::ActiveElement::ActiveElement(float mass) :
        _mass(mass),
        _positionVector(nullptr),
        _positionIndex(0) {

}

const Model::Position &Model::Physics::ActiveElement::position() const {
    return (*_positionVector)[_positionIndex];
}
