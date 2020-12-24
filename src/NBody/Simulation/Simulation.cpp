//
// Created by jackcamp on 12/22/20.
//

#include "Simulation.h"
#include "Drawable.h"

NBody::Simulation::Entity NBody::Simulation::Simulation::createBody() {
    auto entity = create();
    emplace<Position>(entity, 0, 0, 0);
    emplace<Velocity>(entity, 0, 0, 0);
    return entity;
}

void NBody::Simulation::to_json(json &j, const NBody::Simulation::Simulation &s) {

    s.each([&](const auto &entity) {
        json e;

        if (s.has<NBody::Simulation::Position>(entity))
            e["position"] = s.get<NBody::Simulation::Position>(entity);

        if (s.has<NBody::Simulation::Velocity>(entity))
            e["velocity"] = s.get<NBody::Simulation::Velocity>(entity);

        if (s.has<NBody::Simulation::Drawable>(entity))
            e["drawable"] = s.get<NBody::Simulation::Drawable>(entity);

        j["bodies"].push_back(e);
    });
}

void NBody::Simulation::from_json(const json &j, NBody::Simulation::Simulation &s) {
}
