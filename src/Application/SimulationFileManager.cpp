//
// Created by Jackson Campolattaro on 11/4/22.
//

#include "SimulationFileManager.h"

#include <spdlog/spdlog.h>

NBody::SimulationFileManager::SimulationFileManager(NBody::Simulation &simulation) :
        _simulation(simulation) {}

void NBody::SimulationFileManager::open(const Glib::RefPtr<Gio::File> &file) {
    // todo: close currently open file
    _file = file;

    spdlog::debug("Opening JSON file at path \"{}\"", _file->get_path());
    std::ifstream inputFile(_file->get_path());
    json j;
    inputFile >> j;

    auto entities = std::vector<Simulation::entity_type>{j["particles"].size()};
    _simulation.create(entities.begin(), entities.end());

    for (int i = 0; i < entities.size(); ++i) {

        auto p = j["particles"][i];
        Simulation::Particle particle = {_simulation, entities[i]};

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

    _simulation.signal_particles_added.emit(entities);
}

void NBody::SimulationFileManager::save() {
    saveAs(_file);
}

void NBody::SimulationFileManager::saveAs(const Glib::RefPtr<Gio::File> &file) {
    _file = file;
    _simulation.save(*_file);
}
