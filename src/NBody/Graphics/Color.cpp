#include "Color.h"

void NBody::Graphics::to_json(json &j, const NBody::Graphics::Color &c) {
    j["r"] = c.r();
    j["g"] = c.g();
    j["b"] = c.b();
}

void NBody::Graphics::from_json(const json &j, NBody::Graphics::Color &c) {
    c.r() = j["r"].get<float>();
    c.g() = j["g"].get<float>();
    c.b() = j["b"].get<float>();
}
