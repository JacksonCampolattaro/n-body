//
// Created by jackcamp on 12/23/20.
//

#ifndef N_BODY_POSITION_H
#define N_BODY_POSITION_H

#include <glm/vec3.hpp>

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Simulation {

    class Position : public glm::vec3 {};

    void to_json(json &j, const Position &position);

    void from_json(const json &j, Position &position);
}


#endif //N_BODY_POSITION_H