//
// Created by jackcamp on 12/23/20.
//

#ifndef N_BODY_POSITION_H
#define N_BODY_POSITION_H

#include <glm/vec3.hpp>

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Physics {

    class Position : public glm::vec3 {
    public:

        using glm::vec3::vec;

        Position(glm::vec3 vector) : glm::vec3(vector) {};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Position, x, y, z)
    };

}


#endif //N_BODY_POSITION_H
