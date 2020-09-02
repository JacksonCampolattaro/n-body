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

void Model::Entity::setDrawable(const Drawable::Drawable &drawable) {
    _drawable = std::make_shared<Drawable::Drawable>(drawable);
}

std::string Model::Entity::toString() {

    std::stringstream stream;

    stream << "[ entity: ";

    if (_position) {
        stream << _position->toString();
    }

    stream << " ";

    if (_velocity) {
        stream << _velocity->toString();
    }

    if (_drawable) {
        stream << " drawable";
    }

    stream << " ]";

    return stream.str();
}

