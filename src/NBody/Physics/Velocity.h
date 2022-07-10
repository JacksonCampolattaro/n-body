//
// Created by jackcamp on 12/23/20.
//

#ifndef N_BODY_VELOCITY_H
#define N_BODY_VELOCITY_H

#include <glm/vec3.hpp>

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Physics {

    class Velocity : public glm::vec3 {
    public:

        Velocity() : glm::vec3() {};

        Velocity(float x, float y, float z) : glm::vec3(x, y, z) {};

        Velocity(glm::vec3 vector) : glm::vec3(vector) {};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Velocity, x, y, z)
    };

}

#endif //N_BODY_VELOCITY_H
