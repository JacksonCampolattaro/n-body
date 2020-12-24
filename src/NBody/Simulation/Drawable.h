//
// Created by jackcamp on 12/23/20.
//

#ifndef N_BODY_DRAWABLE_H
#define N_BODY_DRAWABLE_H

#include <NBody/Simulation/Color.h>

#include <glm/vec4.hpp>

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Simulation {

    class Drawable {
    public:

        Color _color;
        float _radius;
    };

    void to_json(json &j, const Drawable &drawable);

    void from_json(const json &j, Drawable &drawable);
}


#endif //N_BODY_DRAWABLE_H
