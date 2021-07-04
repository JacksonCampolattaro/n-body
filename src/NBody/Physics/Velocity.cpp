//
// Created by jackcamp on 12/23/20.
//

#include "Velocity.h"

void NBody::Simulation::to_json(json &j, const NBody::Simulation::Velocity &velocity) {

    j = {
            {"dx", velocity.x},
            {"dy", velocity.y},
            {"dz", velocity.z}
    };
}

void NBody::Simulation::from_json(const json &j, NBody::Simulation::Velocity &velocity) {

    j.at("dx").get_to(velocity.x);
    j.at("dy").get_to(velocity.y);
    j.at("dz").get_to(velocity.z);
}
