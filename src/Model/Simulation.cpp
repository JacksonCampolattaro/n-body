//
// Created by jackcamp on 4/17/20.
//

#include "Simulation.h"

void Model::Simulation::addEntity(const Entity &entity) {

    spdlog::debug("Adding an entity to the simulation");

    _entities.push_back(entity);

    _positions.emplace_back(*_entities.back()._position);
    _entities.back()._position = std::make_shared<Position>(_positions.back());

    _velocities.emplace_back(*_entities.back()._velocity);
    _entities.back()._velocity = std::make_shared<Velocity>(_velocities.back());

    if (_entities.back()._drawable) {
        _drawables.emplace_back(_entities.back()._drawable);
        _entities.back()._drawable = _drawables.back();
        _drawables.back()->_position = _entities.back()._position;
    }

    spdlog::trace(_entities.back().toString());
}

void Model::Simulation::update() {

    for (int i = 0; i < _positions.size(); ++i) {
        spdlog::trace("old position: " + _positions[i].toString());
        _positions[i].x += _velocities[i].x;
        _positions[i].y += _velocities[i].y;
        _positions[i].z += _velocities[i].z;
        spdlog::trace("new position: " + _positions[i].toString());
    }
}
