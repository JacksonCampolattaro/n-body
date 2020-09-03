//
// Created by jackcamp on 4/17/20.
//

#include "Rule.h"

Model::Physics::Rule::Rule() :
    _G(1.0),
    _t(1.0) {
}

void
Model::Physics::Rule::operator()(const Model::Physics::ActiveElement &active, Model::Physics::PassiveElement &passive) {

    (*passive._velocityVector)[passive._velocityIndex].x += 0.5;
}
