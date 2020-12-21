//
// Created by jackcamp on 9/2/20.
//

#include "ActiveElement.h"


Model::Physics::ActiveElement::ActiveElement(float mass) :
        _position(nullptr),
        _mass(mass) {

}

const Model::Position &Model::Physics::ActiveElement::position() const {
    return *_position;
}
