//
// Created by jackcamp on 4/17/20.
//

#include "Entity.h"

Model::Entity::Entity() {}

Model::Entity *Model::Entity::setPosition(Model::Position position) {
    _position = std::make_shared<Model::Position>(position);
    return this;
}

Model::Entity *Model::Entity::setVelocity(Model::Velocity velocity) {
    _velocity = std::make_shared<Model::Velocity>(velocity);
    return this;
}

std::string Model::Entity::toString() {

    std::stringstream stream;

    stream << "[ entity: ";

    if (nullptr != _position) {
        stream << _position->toString();
    }

    stream << " ";

    if (nullptr != _velocity) {
        stream << _velocity->toString();
    }

    stream << " ]";

    return stream.str();
}

