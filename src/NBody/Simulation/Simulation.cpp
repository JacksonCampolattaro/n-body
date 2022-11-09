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

    signal_particles_added.emit({particle.entity()});
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
    emplace_or_replace<NBody::Physics::Mass>(mass);
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

        if (s.all_of<NBody::Physics::Mass>(entity))
            e["mass"] = s.get<NBody::Physics::Mass>(entity);

        e["active"] = s.all_of<NBody::Physics::ActiveTag>(entity);

        e["passive"] = s.all_of<NBody::Physics::PassiveTag>(entity);

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

        if (!p.contains("active") || p["active"].get<bool>())
            particle.emplace<NBody::Physics::ActiveTag>();

        if (!p.contains("passive") || p["passive"].get<bool>())
            particle.emplace<NBody::Physics::PassiveTag>();

        if (p.contains("color"))
            particle.setColor(p["color"].get<NBody::Graphics::Color>());

        if (p.contains("sphere"))
            particle.setSphere(p["sphere"].get<NBody::Graphics::Sphere>());

        // Notify any watchers that this particle has new data
        if (particle.all_of<sigc::signal<void()>>())
            particle.get<sigc::signal<void()>>().emit();
    }

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

std::vector<NBody::Entity> NBody::Simulation::validEntities() {
    std::vector<NBody::Entity> alive;
    each([&](auto e) {
        alive.emplace_back(e);
    });
    return alive;
}

void NBody::Simulation::removeParticle(NBody::Entity entity) {

    assert(valid(entity));
    destroy(entity);
    signal_particles_removed.emit({entity});
    signal_changed.emit();
}

std::size_t NBody::Simulation::particleCount() const {
    return size<Physics::Position>();
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

    auto masses = view<const Physics::Mass>();
    masses.each([&](const auto &mass) {
        totalMass += mass.mass();
    });

    return totalMass;
}

NBody::Physics::Position NBody::Simulation::centerOfMass() const {
    Physics::Position average;
    float totalMass = 0;

    auto massesAndPositions = view<const Physics::Mass, const Physics::Position>();
    massesAndPositions.each([&](const auto &mass, const auto &position) {
        average = average + (position * mass.mass());
        totalMass += mass.mass();
    });

    return average / totalMass;
}

std::size_t NBody::Simulation::interactionCount() const {
    return view<const Physics::PassiveTag>().size() * view<const Physics::ActiveTag>().size();
}
