//
// Created by jackcamp on 12/22/20.
//

#include <spdlog/spdlog.h>
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

NBody::Simulation::Particle &NBody::Simulation::Particle::setMass(const float &mass) {
    emplace_or_replace<NBody::Physics::ActiveMass>(mass);
    emplace_or_replace<NBody::Physics::PassiveMass>(mass);
    return *this;
}

NBody::Simulation::Particle &NBody::Simulation::Particle::setActiveMass(const NBody::Physics::ActiveMass &mass) {
    emplace_or_replace<NBody::Physics::ActiveMass>(mass);
    return *this;
}

NBody::Simulation::Particle &NBody::Simulation::Particle::setPassiveMass(const NBody::Physics::PassiveMass &mass) {
    emplace_or_replace<NBody::Physics::PassiveMass>(mass);
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
    std::scoped_lock l(s.mutex);

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

    spdlog::debug("Serialized {} particles", j["particles"].size());
}

void NBody::from_json(const json &j, NBody::Simulation &s) {
    std::scoped_lock l(s.mutex);

    for (const auto &p: j["particles"]) {
        auto particle = s.newParticle();

        if (p.contains("position"))
            particle.setPosition(p["position"].get<NBody::Physics::Position>());

        if (p.contains("velocity"))
            particle.setVelocity(p["velocity"].get<NBody::Physics::Velocity>());

        if (p.contains("mass"))
            particle.setMass(p["mass"].get<float>());

        if (p.contains("color"))
            particle.setColor(p["color"].get<NBody::Graphics::Color>());

        if (p.contains("sphere"))
            particle.setSphere(p["sphere"].get<NBody::Graphics::Sphere>());
    }

    s.signal_particles_added.emit(j["particles"].size());
    s.signal_changed.emit();

    spdlog::debug("Read {} particles", j["particles"].size());
}

void NBody::Simulation::save(Gio::File &destination) const {
    spdlog::debug("Writing JSON file to path \"{}\"", destination.get_path());
    std::ofstream outputFile{destination.get_path()};
    outputFile << (*this);
}

void NBody::Simulation::load(Gio::File &source) {
    spdlog::debug("Loading JSON file from path \"{}\"", source.get_path());
    std::ifstream inputFile(source.get_path());
     inputFile >> (*this);
}

NBody::Physics::Position NBody::Simulation::averagePosition() const {
    Physics::Position average;

    auto positions = view<const Physics::Position>();
    positions.each([&](const auto &position) {
        average = average + position;
    });

    return average / (float) positions.size();
}

float NBody::Simulation::totalMass() const {
    float totalMass = 0;

    auto masses = view<const Physics::PassiveMass>();
    masses.each([&](const auto &mass) {
       totalMass += mass.mass();
    });

    return totalMass;
}

NBody::Physics::Position NBody::Simulation::centerOfMass() const {
    Physics::Position average;
    float totalMass = 0;

    auto massesAndPositions = view<const Physics::PassiveMass, const Physics::Position>();
    massesAndPositions.each([&](const auto &mass, const auto &position) {
        average = average + (position * mass.mass());
        totalMass += mass.mass();
    });

    return average / totalMass;
}

std::size_t NBody::Simulation::interactionCount() const {
    return view<const Physics::PassiveMass>().size() * view<const Physics::ActiveMass>().size();
}
