//
// Created by jackcamp on 6/18/19.
//

#include <cereal/cereal.hpp>
#include "Position.h"

std::string Position::toString() {
    return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ", " + std::to_string(this->z) + ")";
}

