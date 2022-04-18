//
// Created by jackcamp on 4/12/22.
//

#include "PassiveMass.h"

bool NBody::Physics::PassiveMass::operator==(const NBody::Physics::PassiveMass &other) const {
    return mass() == other.mass();
}

void NBody::Physics::to_json(json &j, const NBody::Physics::PassiveMass &m) {
    j = m.mass();
}

void NBody::Physics::from_json(const json &j, NBody::Physics::PassiveMass &m) {
    m.mass() = j.get<float>();
}
