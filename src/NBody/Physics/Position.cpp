//
// Created by jackcamp on 12/23/20.
//

#include "Position.h"

void NBody::Simulation::to_json(json &j, const NBody::Simulation::Position &position) {

    j = {
            {"x", position.x},
            {"y", position.y},
            {"z", position.z}
    };
}

void NBody::Simulation::from_json(const json &j, NBody::Simulation::Position &position) {

    j.at("x").get_to(position.x);
    j.at("y").get_to(position.y);
    j.at("z").get_to(position.z);
}
