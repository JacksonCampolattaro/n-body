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
