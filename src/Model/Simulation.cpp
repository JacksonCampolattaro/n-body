//
// Created by jackcamp on 4/17/20.
//

#include "Simulation.h"

void Model::Simulation::addEntity(std::shared_ptr<Entity> entity) {

    spdlog::debug("Adding an entity to the simulation");

    _entities.push_back(entity);

    if (nullptr != entity->_position) {
        _positions.push_back(*entity->_position);
        entity->_position.reset(&_positions.back());
    }

    if (nullptr != entity->_velocity) {
        _velocities.push_back(*entity->_velocity);
        entity->_velocity.reset(&_velocities.back());
    }
}
