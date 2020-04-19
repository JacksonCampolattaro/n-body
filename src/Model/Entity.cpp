//
// Created by jackcamp on 4/17/20.
//

#include "Entity.h"

Model::Entity::Entity() :
        _position(std::make_shared<Position>(0, 0, 0)),
        _velocity(std::make_shared<Velocity>(0, 0, 0)) {}

void Model::Entity::setPosition(const Position &position) {
    _position = std::make_shared<Model::Position>(position);
}

void Model::Entity::setVelocity(const Velocity &velocity) {
    _velocity = std::make_shared<Model::Velocity>(velocity);
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

