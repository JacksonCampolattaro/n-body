//
// Created by jackcamp on 4/17/20.
//

#include "Simulation.h"


void Model::Simulation::update() {

    for (int i = 0; i < _positions.size(); ++i) {
        spdlog::trace("old position: " + _positions[i].toString());
        _positions[i].x += _velocities[i].x;
        _positions[i].y += _velocities[i].y;
        _positions[i].z += _velocities[i].z;
        spdlog::trace("new position: " + _positions[i].toString());
        spdlog::trace("drawable position: " + _drawables[i]->_position->toString());
        spdlog::trace("entity position: " + _entities[i]._position->toString());
    }
}

Model::Entity &Model::Simulation::addEntity(Model::Position position, Model::Velocity velocity) {

    _positions.emplace_back(position);
    _velocities.emplace_back(velocity);
    _entities.emplace_back();

    _entities.back()._position = &_positions.back();
    _entities.back()._velocity = &_velocities.back();

    return _entities.back();
}

Model::Entity &Model::Simulation::addEntity(Model::Position position, Model::Velocity velocity,
                                            const Model::Drawable::Drawable &drawable) {

    _drawables.emplace_back(std::make_shared<Drawable::Drawable>(drawable));

    addEntity(position, velocity)._drawable = _drawables.back();
    _drawables.back()->_position = &_positions.back();

    return _entities.back();
}
