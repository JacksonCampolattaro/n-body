//
// Created by jackcamp on 4/17/20.
//

#include "Simulation.h"


void Model::Simulation::update() {

    spdlog::trace("Update");

    // Kick
    for (auto &active : _activeElements) {
        for (auto &passive : _passiveElements) {
            if (passive.position() != active.position())
                _rule(active, passive);
        }
    }

    // Drift
    for (int i = 0; i < _positions.size(); ++i)
        _positions[i] = _positions[i] + _velocities[i];
}

Model::Entity &Model::Simulation::addEntity(Model::Position position, Model::Velocity velocity) {

    _entities.push_back(Entity(*this, position, velocity));
    return _entities.back();
}

void Model::Simulation::addEntity(Model::Position position, Model::Velocity velocity,
                                            const Model::Drawable::Drawable &drawable) {

    addEntity(position, velocity);
}

void Model::Simulation::addEntity(Model::Position position, Model::Velocity velocity,
                                  const Model::Drawable::Drawable &drawable,
                                  float mass) {

    addEntity(position, velocity, drawable);

    _passiveElements.emplace_back(mass);
    _passiveElements.back()._positionVector = &_positions;
    _passiveElements.back()._positionIndex = _positions.size() - 1;
    _passiveElements.back()._velocityVector = &_velocities;
    _passiveElements.back()._velocityIndex = _velocities.size() - 1;

    _activeElements.emplace_back(mass);
    _activeElements.back()._positionVector = &_positions;
    _activeElements.back()._positionIndex = _positions.size() - 1;
}