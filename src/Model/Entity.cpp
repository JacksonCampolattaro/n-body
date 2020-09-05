//
// Created by jackcamp on 9/3/20.
//

#include "Entity.h"

#include "Simulation.h"

Model::Entity::Entity(Model::Simulation &simulation) : _simulation(simulation) {

    setPosition({0, 0, 0});
    setVelocity({0, 0, 0});
}

Model::Entity &Model::Entity::setPosition(const Model::Position &position) {
    _simulation._positions.emplace_back(position);
    _position = &_simulation._positions.back();
    return *this;
}

Model::Entity &Model::Entity::setVelocity(const Model::Velocity &velocity) {
    _simulation._velocities.emplace_back(velocity);
    _velocity = &_simulation._velocities.back();
    return *this;
}

Model::Entity &Model::Entity::setDrawable(const Model::Drawable::Drawable &drawable) {
    _simulation._drawables.emplace_back(drawable);
    _drawable = &_simulation._drawables.back();
    _drawable->_position = _position;
    return *this;
}

Model::Entity &Model::Entity::setPassiveElement(const Model::Physics::PassiveElement &passiveElement) {
    _simulation._passiveElements.emplace_back(passiveElement);
    _passiveElement = &_simulation._passiveElements.back();
    _passiveElement->_position = _position;
    _passiveElement->_velocity = _velocity;
    return *this;
}

Model::Entity &Model::Entity::setActiveElement(const Model::Physics::ActiveElement &activeElement) {
    _simulation._activeElements.emplace_back(activeElement);
    _activeElement = &_simulation._activeElements.back();
    _activeElement->_position = _position;
    return *this;
}
