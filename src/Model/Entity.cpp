//
// Created by jackcamp on 4/17/20.
//

#include "Entity.h"

Model::Entity::Entity() {}

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

