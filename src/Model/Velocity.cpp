//
// Created by jackcamp on 4/17/20.
//

#include "Velocity.h"

std::string Model::Velocity::toString() const {
    return "<" + std::to_string(this->x) + ", " + std::to_string(this->y) + ", " + std::to_string(this->z) + ">";
}

std::ostream &Model::operator<<(std::ostream &os, const Model::Velocity &body) {

    os << "< "
       << body.x << ", "
       << body.y << ", "
       << body.z << " >";

    return os;
}
