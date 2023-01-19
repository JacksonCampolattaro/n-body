//
// Created by Jackson Campolattaro on 1/17/23.
//

#ifndef N_BODY_ACCELERATION_H
#define N_BODY_ACCELERATION_H

#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Physics {

    class Acceleration : public glm::vec3 {
    public:

        Acceleration() : glm::vec3() {};

        Acceleration(float x, float y, float z) : glm::vec3(x, y, z) {};

        Acceleration(glm::vec3 vector) : glm::vec3(vector) {};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Acceleration, x, y, z)
    };

}

#endif //N_BODY_ACCELERATION_H
