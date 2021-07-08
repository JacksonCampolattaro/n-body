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

        Position() : glm::vec3() {};

        Position(float x, float y, float z) : glm::vec3(x, y, z) {};

        explicit Position(glm::vec3 vector) : glm::vec3(vector) {};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Position, x, y, z)
    };

}


#endif //N_BODY_POSITION_H
