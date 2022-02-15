//
// Created by jackcamp on 12/22/20.
//

#include "Simulation.h"

NBody::Simulation::Particle NBody::Simulation::newParticle() {

    // Create a new empty particle
    Particle particle = {*this, create()};
    particle.setPosition({0, 0, 0})
            .setVelocity({0, 0, 0});

    return particle;
}

NBody::Simulation::Particle &NBody::Simulation::Particle::setPosition(const NBody::Physics::Position &position) {
    emplace_or_replace<NBody::Physics::Position>(position);
    return *this;
}

NBody::Simulation::Particle &NBody::Simulation::Particle::setVelocity(const NBody::Physics::Velocity &velocity) {
    emplace_or_replace<NBody::Physics::Velocity>(velocity);
    return *this;
}

NBody::Simulation::Particle &NBody::Simulation::Particle::setMass(const NBody::Physics::ActiveMass &mass) {
    emplace_or_replace<NBody::Physics::ActiveMass>(mass);
    return *this;
}

NBody::Simulation::Particle &NBody::Simulation::Particle::setColor(const NBody::Graphics::Color &color) {
    emplace_or_replace<NBody::Graphics::Color>(color);
    return *this;
}

NBody::Simulation::Particle &NBody::Simulation::Particle::setSphere(const NBody::Graphics::Sphere &sphere) {
    emplace_or_replace<NBody::Graphics::Sphere>(sphere);
    return *this;
}

void NBody::to_json(json &j, const NBody::Simulation &s) {

    s.each([&](const auto &entity) {
        json e;

        if (s.all_of<NBody::Physics::Position>(entity))
            e["position"] = s.get<NBody::Physics::Position>(entity);

        if (s.all_of<NBody::Physics::Velocity>(entity))
            e["velocity"] = s.get<NBody::Physics::Velocity>(entity);

        if (s.all_of<NBody::Physics::ActiveMass>(entity))
            e["mass"] = s.get<NBody::Physics::ActiveMass>(entity);

        if (s.all_of<NBody::Graphics::Color>(entity))
            e["color"] = s.get<NBody::Graphics::Color>(entity);

        if (s.all_of<NBody::Graphics::Sphere>(entity))
            e["sphere"] = s.get<NBody::Graphics::Sphere>(entity);

        j["particles"].push_back(e);
    });
}

void NBody::from_json(const json &j, NBody::Simulation &s) {

    for (const auto &p: j["particles"]) {
        auto particle = s.newParticle();

        if (p.contains("position"))
            particle.setPosition(p["position"].get<NBody::Physics::Position>());

        if (p.contains("velocity"))
            particle.setVelocity(p["velocity"].get<NBody::Physics::Velocity>());

        if (p.contains("mass"))
            particle.setMass(p["mass"].get<NBody::Physics::ActiveMass>());

        if (p.contains("color"))
            particle.setColor(p["color"].get<NBody::Graphics::Color>());

        if (p.contains("sphere"))
            particle.setSphere(p["sphere"].get<NBody::Graphics::Sphere>());
    }
}
