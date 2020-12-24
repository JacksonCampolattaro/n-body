//
// Created by jackcamp on 12/23/20.
//

#ifndef N_BODY_VELOCITY_H
#define N_BODY_VELOCITY_H

#include <glm/vec3.hpp>

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Simulation {

    class Velocity : public glm::vec3 {
    public:

        Velocity(float x, float y, float z) : glm::vec3(x, y, z) {};

        Velocity(glm::vec3 vector) : glm::vec3(vector) {};
    };

    void to_json(json &j, const Velocity &velocity);

    void from_json(const json &j, Velocity &velocity);

}

#endif //N_BODY_VELOCITY_H
