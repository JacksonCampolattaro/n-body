//
// Created by jackcamp on 12/22/20.
//

#include "Simulation.h"

NBody::Simulation::Entity NBody::Simulation::Simulation::createBody() {
    auto entity = create();
    emplace<Position>(entity, 0, 0, 0);
    emplace<Velocity>(entity, 0, 0, 0);
    return entity;
}

json NBody::Simulation::Simulation::to_json() const {
    json j;

    this->each([&](const auto &entity) {
        json e;

        if (this->has<NBody::Simulation::Position>(entity))
            e["position"] = this->get<NBody::Simulation::Position>(entity);

        if (this->has<NBody::Simulation::Velocity>(entity))
            e["velocity"] = this->get<NBody::Simulation::Velocity>(entity);

        j["bodies"].push_back(e);
    });

    return j;
}

void NBody::Simulation::Simulation::from_json(const json &j) {

}
