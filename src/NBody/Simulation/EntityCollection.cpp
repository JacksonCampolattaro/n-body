//
// Created by jackcamp on 12/22/20.
//

#include "EntityCollection.h"

void NBody::Simulation::to_json(json &j, const NBody::Simulation::EntityCollection &entityCollection) {

    auto view = entityCollection.view<const Position>();

    for (auto entity : view) {
        json e;

        if (entityCollection.has<NBody::Simulation::Position>(entity))
            e["position"] = entityCollection.get<NBody::Simulation::Position>(entity);

        j.push_back(e);
    }
}

void NBody::Simulation::from_json(const json &j, NBody::Simulation::EntityCollection &entityCollection) {

}
