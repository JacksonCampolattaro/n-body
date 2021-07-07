#ifndef N_BODY_COLOR_H
#define N_BODY_COLOR_H

#include <Magnum/Magnum.h>
#include <Magnum/Math/Color.h>

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Graphics {

    class Color : public Magnum::Color3 {};

    void to_json(json &j, const Color &c);

    void from_json(const json &j, Color &c);

}

#endif //N_BODY_COLOR_H
