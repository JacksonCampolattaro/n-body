//
// Created by Jackson Campolattaro on 1/17/23.
//

#ifndef N_BODY_FORCE_H
#define N_BODY_FORCE_H

#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Physics {

    class Force : public glm::vec3 {
    public:

        Force() : glm::vec3() {};

        Force(float x, float y, float z) : glm::vec3(x, y, z) {};

        Force(glm::vec3 vector) : glm::vec3(vector) {};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Force, x, y, z)
    };

}

#endif //N_BODY_FORCE_H
