#include "Color.h"

void NBody::Graphics::to_json(json &j, const NBody::Graphics::Color &c) {
    j["r"] = c.r();
    j["g"] = c.g();
    j["b"] = c.b();
}

void NBody::Graphics::from_json(const json &j, NBody::Graphics::Color &c) {
    c.r() = std::min(j["r"].get<float>(), 1.0f);
    c.g() = std::min(j["g"].get<float>(), 1.0f);
    c.b() = std::min(j["b"].get<float>(), 1.0f);
}
