//
// Created by jackcamp on 12/23/20.
//

#ifndef N_BODY_COLOR_H
#define N_BODY_COLOR_H

#include <glm/vec4.hpp>

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Simulation {

    class Color : public glm::vec4 {
    public:

        Color(float r, float g, float b, float a) : glm::vec4(r, g, b, a) {};

        Color(glm::vec4 vector) : glm::vec4(vector) {};
    };

    void to_json(json &j, const Color &color);

    void from_json(const json &j, Color &color);
}


#endif //N_BODY_COLOR_H
