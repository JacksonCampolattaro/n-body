//
// Created by jackcamp on 12/27/20.
//

#include "ActiveElement.h"

#include "PassiveElement.h"


NBody::Simulation::Physics::PassiveElement::PassiveElement(float mass) : _mass(mass) {}

float &NBody::Simulation::Physics::PassiveElement::mass() {
    return _mass;
}

const float &NBody::Simulation::Physics::PassiveElement::mass() const {
    return _mass;
}

bool NBody::Simulation::Physics::operator==(const NBody::Simulation::Physics::PassiveElement &left,
                                            const NBody::Simulation::Physics::PassiveElement &right) {
    return left.mass() == right.mass();
}

void NBody::Simulation::Physics::to_json(json &j, const NBody::Simulation::Physics::PassiveElement &passiveElement) {
    j["mass"] = passiveElement.mass();
}

void NBody::Simulation::Physics::from_json(const json &j, NBody::Simulation::Physics::PassiveElement &passiveElement) {
    j.at("mass").get_to(passiveElement.mass());
}
