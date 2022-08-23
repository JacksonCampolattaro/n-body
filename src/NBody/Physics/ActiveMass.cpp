//
// Created by jackcamp on 7/8/21.
//

#include "Mass.h"

bool NBody::Physics::Mass::operator==(const NBody::Physics::Mass &other) const {
    return mass() == other.mass();
}

void NBody::Physics::to_json(json &j, const NBody::Physics::Mass &m) {
    j = m.mass();
}

void NBody::Physics::from_json(const json &j, NBody::Physics::Mass &m) {
    m.mass() = j.get<float>();
}
