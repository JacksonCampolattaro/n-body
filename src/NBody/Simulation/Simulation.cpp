//
// Created by jackcamp on 12/22/20.
//

#include "Simulation.h"

NBody::Simulation::Entity NBody::Simulation::Simulation::newEntity() {
    auto entity = _entityCollection.create();
    _entityCollection.emplace<Position>(entity, glm::vec3{0, 0, 0});
    return entity;
}
