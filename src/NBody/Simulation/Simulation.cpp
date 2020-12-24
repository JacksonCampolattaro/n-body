//
// Created by jackcamp on 12/22/20.
//

#include "Simulation.h"

NBody::Simulation::Entity NBody::Simulation::Simulation::newEntity() {
    auto entity = _entityCollection.create();
    _entityCollection.emplace<Position>(entity, glm::vec3{0, 0, 0});
    _entityCollection.emplace<Velocity>(entity, glm::vec3{0, 0, 0});
    return entity;
}

NBody::Simulation::EntityCollection &NBody::Simulation::Simulation::data() {
    return _entityCollection;
}

const NBody::Simulation::EntityCollection &NBody::Simulation::Simulation::data() const {
    return _entityCollection;
}

void NBody::Simulation::to_json(json &j, const NBody::Simulation::Simulation &s) {
    j["bodies"] = {s.data()};
}

void NBody::Simulation::from_json(const json &j, NBody::Simulation::Simulation &s) {
    j.at("bodies").get_to(s.data());
}
