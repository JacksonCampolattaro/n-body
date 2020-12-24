//
// Created by jackcamp on 12/22/20.
//

#include "Simulation.h"

void NBody::Simulation::to_json(json &j, const NBody::Simulation::Simulation &s) {

    auto view = s.view<const Position>();

    for (auto entity : view) {
        json e;

        if (s.has<NBody::Simulation::Position>(entity))
            e["position"] = s.get<NBody::Simulation::Position>(entity);

        if (s.has<NBody::Simulation::Velocity>(entity))
            e["velocity"] = s.get<NBody::Simulation::Velocity>(entity);

        j.push_back(e);
    }
}

void NBody::Simulation::from_json(const json &j, NBody::Simulation::Simulation &s) {
}
