//
// Created by jackcamp on 7/8/21.
//

#include "ActiveMass.h"

bool NBody::Physics::ActiveMass::operator==(const NBody::Physics::ActiveMass &other) const {
    return mass() == other.mass();
}

void NBody::Physics::to_json(json &j, const NBody::Physics::ActiveMass &m) {
    j = m.mass();
}

void NBody::Physics::from_json(const json &j, NBody::Physics::ActiveMass &m) {
    m.mass() = j.get<float>();
}
