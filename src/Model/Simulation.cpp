//
// Created by jackcamp on 4/17/20.
//

#include "Simulation.h"

void Model::Simulation::addEntity(const Entity &entity) {

    spdlog::debug("Adding an entity to the simulation");

    _entities.push_back(entity);

    _positions.emplace_back(*_entities.back()._position);
    _entities.back()._position.reset(&_positions.back());

    _velocities.emplace_back(*_entities.back()._position);
    _entities.back()._velocity.reset(&_velocities.back());
}
