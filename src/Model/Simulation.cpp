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

    // Alert the renderer
    signal_update_complete.emit();
}

Model::Entity &Model::Simulation::addEntity(Model::Position position, Model::Velocity velocity) {

    _entities.push_back(Entity(*this, position, velocity));
    return _entities.back();
}
