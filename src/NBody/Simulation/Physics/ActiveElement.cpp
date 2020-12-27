//
// Created by jackcamp on 9/2/20.
//

#include "ActiveElement.h"

NBody::Simulation::Physics::ActiveElement::ActiveElement(float mass) : _mass(mass) {}

const float &NBody::Simulation::Physics::ActiveElement::mass() const {
    return _mass;
}

float &NBody::Simulation::Physics::ActiveElement::mass() {
    return _mass;
}

bool NBody::Simulation::Physics::operator==(const NBody::Simulation::Physics::ActiveElement &left,
                                            const NBody::Simulation::Physics::ActiveElement &right) {
    return left.mass() == right.mass();
}

void NBody::Simulation::Physics::to_json(json &j, const NBody::Simulation::Physics::ActiveElement &activeElement) {
    j["mass"] = activeElement.mass();
}

void NBody::Simulation::Physics::from_json(const json &j, NBody::Simulation::Physics::ActiveElement &activeElement) {
    j.at("mass").get_to(activeElement.mass());
}
