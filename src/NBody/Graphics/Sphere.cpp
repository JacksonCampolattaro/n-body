//
// Created by jackcamp on 7/6/21.
//

#include "Sphere.h"

GL::Mesh NBody::Graphics::Sphere::mesh() {
    return MeshTools::compile(Primitives::icosphereSolid(2));
}

void NBody::Graphics::to_json(json &j, const NBody::Graphics::Sphere &s) {
    j["radius"] = s.radius();
}

void NBody::Graphics::from_json(const json &j, NBody::Graphics::Sphere &s) {
    s.radius() = j["radius"].get<float>();
}
