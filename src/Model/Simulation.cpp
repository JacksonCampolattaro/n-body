//
// Created by jackcamp on 4/17/20.
//

#include "Simulation.h"


void Model::Simulation::update() {

    for (int i = 0; i < _positions.size(); ++i) {
        _positions[i] = _positions[i] + _velocities[i];
    }
}

void Model::Simulation::addEntity(Model::Position position, Model::Velocity velocity) {

    _positions.emplace_back(position);
    _velocities.emplace_back(velocity);
}

void Model::Simulation::addEntity(Model::Position position, Model::Velocity velocity,
                                            const Model::Drawable::Drawable &drawable) {

    addEntity(position, velocity);

    _drawables.push_back(std::make_shared<Drawable::Drawable>(drawable));
    _drawables.back()->_positionVector = &_positions;
    _drawables.back()->_positionIndex = _positions.size() - 1;
}
