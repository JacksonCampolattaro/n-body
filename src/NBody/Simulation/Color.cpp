//
// Created by jackcamp on 12/23/20.
//

#include "Color.h"

void NBody::Simulation::to_json(json &j, const NBody::Simulation::Color &color) {

    j = {
            {"r", color.r},
            {"g", color.g},
            {"b", color.b},
            {"a", color.a},
    };
}

void NBody::Simulation::from_json(const json &j, NBody::Simulation::Color &color) {

}
