//
// Created by jackcamp on 4/17/20.
//

#include "Position.h"

std::string Model::Position::toString() const {
    return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ", " + std::to_string(this->z) + ")";
}

std::ostream &Model::operator<<(std::ostream &os, const Model::Position &position) {

    os << "( "
       << position.x << ", "
       << position.y << ", "
       << position.z << " )";

    return os;
}

std::istream &Model::operator>>(std::istream &in, Model::Position &position) {

    char _;
    in >> _ >> position.x >> _ >> position.y >> _ >> position.z >> _;
    return in;
}
