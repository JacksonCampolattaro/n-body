//
// Created by jackcamp on 12/22/20.
//

#include "Simulation.h"

NBody::Simulation::Entity NBody::Simulation::Simulation::createBody() {
    auto entity = create();
    emplace<Position>(entity, 0, 0, 0);
    emplace<Velocity>(entity, 0, 0, 0);
    return entity;
}

void NBody::Simulation::to_json(json &j, const NBody::Simulation::Simulation &s) {

    s.each([&](const auto &entity) {
        json e;

        if (s.has<NBody::Simulation::Position>(entity))
            e["position"] = s.get<NBody::Simulation::Position>(entity);

        if (s.has<NBody::Simulation::Velocity>(entity))
            e["velocity"] = s.get<NBody::Simulation::Velocity>(entity);

        if (s.has<NBody::Graphics::Color>(entity))
            e["color"] = s.get<NBody::Graphics::Color>(entity);

        if (s.has<NBody::Graphics::Sphere>(entity))
            e["sphere"] = s.get<NBody::Graphics::Sphere>(entity);

        j["bodies"].push_back(e);
    });
}

void NBody::Simulation::from_json(const json &j, NBody::Simulation::Simulation &s) {

    for (const auto &b : j["bodies"]) {
        auto body = s.createBody();

        if (b.contains("position"))
            s.emplace_or_replace<Position>(body, b["position"].get<Position>());

        if (b.contains("velocity"))
            s.emplace_or_replace<Velocity>(body, b["velocity"].get<Velocity>());

        if (b.contains("color"))
            s.emplace_or_replace<NBody::Graphics::Color>(body, b["color"].get<NBody::Graphics::Color>());

        if (b.contains("sphere"))
            s.emplace_or_replace<NBody::Graphics::Sphere>(body, b["sphere"].get<NBody::Graphics::Sphere>());
    }
}
