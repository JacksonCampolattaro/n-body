//
// Created by jackcamp on 12/22/20.
//

#include "Simulation.h"

NBody::Simulation::Simulation::Body NBody::Simulation::Simulation::createBody() {
    auto body = Body(create(), *this);
    body.setPosition({0, 0, 0});
    body.setVelocity({0, 0, 0});
    return body;
}

void NBody::Simulation::to_json(json &j, const NBody::Simulation::Simulation &s) {

    s.each([&](const auto &entity) {
        json e;

        if (s.has<NBody::Physics::Position>(entity))
            e["position"] = s.get<NBody::Physics::Position>(entity);

        if (s.has<NBody::Physics::Velocity>(entity))
            e["velocity"] = s.get<NBody::Physics::Velocity>(entity);

        if (s.has<NBody::Physics::ActiveMass>(entity))
            e["mass"] = s.get<NBody::Physics::ActiveMass>(entity);

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
            body.setPosition(b["position"].get<NBody::Physics::Position>());

        if (b.contains("velocity"))
            body.setVelocity(b["velocity"].get<NBody::Physics::Velocity>());

        if (b.contains("mass"))
            body.setMass(b["mass"].get<NBody::Physics::ActiveMass>());

        if (b.contains("color"))
            body.setColor(b["color"].get<NBody::Graphics::Color>());

        if (b.contains("sphere"))
            body.setSphere(b["sphere"].get<NBody::Graphics::Sphere>());
    }
}

NBody::Simulation::Simulation::Body &
NBody::Simulation::Simulation::Body::setPosition(const NBody::Physics::Position &position) {
    _simulation.emplace_or_replace<NBody::Physics::Position>(_entity, position);
    return *this;
}

NBody::Simulation::Simulation::Body &
NBody::Simulation::Simulation::Body::setVelocity(const NBody::Physics::Velocity &velocity) {
    _simulation.emplace_or_replace<NBody::Physics::Velocity>(_entity, velocity);
    return *this;
}

NBody::Simulation::Simulation::Body &
NBody::Simulation::Simulation::Body::setMass(const NBody::Physics::ActiveMass &mass) {
    _simulation.emplace_or_replace<NBody::Physics::ActiveMass>(_entity, mass);
    return *this;
}

NBody::Simulation::Simulation::Body &
NBody::Simulation::Simulation::Body::setColor(const NBody::Graphics::Color &color) {
    _simulation.emplace_or_replace<NBody::Graphics::Color>(_entity, color);
    return *this;
}

NBody::Simulation::Simulation::Body &
NBody::Simulation::Simulation::Body::setSphere(const NBody::Graphics::Sphere &sphere) {
    _simulation.emplace_or_replace<NBody::Graphics::Sphere>(_entity, sphere);
    return *this;
}
