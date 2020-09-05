//
// Created by jackcamp on 9/3/20.
//

#include "Entity.h"

#include "Simulation.h"

Model::Entity::Entity(Simulation &simulation, Position &position, Velocity &velocity) :
        _simulation(simulation),
        _position(nullptr),
        _velocity(nullptr) {

    _simulation._positions.emplace_back(position);
    _position = &_simulation._positions.back();

    _simulation._velocities.emplace_back(velocity);
    _velocity = &_simulation._velocities.back();
}

Model::Entity &Model::Entity::addDrawable(Color3 color, float radius) {

    _simulation._drawables.emplace_back(_position, color, radius);
    _drawable = &_simulation._drawables.back();
    _simulation.signal_num_drawables_changed.emit(_simulation._drawables);
    return *this;
}

Model::Entity &Model::Entity::addPassiveElement(float mass) {

    _simulation._passiveElements.emplace_back(_position, _velocity, mass);
    _passiveElement = &_simulation._passiveElements.back();
    return *this;
}

Model::Entity &Model::Entity::addActiveElement(float mass) {

    _simulation._activeElements.emplace_back(_position, mass);
    _activeElement = &_simulation._activeElements.back();
    return *this;
}
