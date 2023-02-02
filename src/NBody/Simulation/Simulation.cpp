//
// Created by jackcamp on 12/22/20.
//

#include "Simulation.h"

#include <spdlog/spdlog.h>
#include <glm/common.hpp>
#include <climits>

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

NBody::Simulation::Particle &NBody::Simulation::Particle::setAcceleration(const Physics::Acceleration &acceleration) {
    emplace_or_replace<NBody::Physics::Acceleration>(acceleration);
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
    return view<const Physics::Acceleration>().size() * view<const Physics::Mass>().size();
}

NBody::BoundingBox NBody::Simulation::boundingBox() const {
    BoundingBox bbox;
    auto passiveParticles = view<const Physics::Position>();
    passiveParticles.each([&](const auto &position) {
        bbox.min() = glm::min((glm::vec3) bbox.min(), (glm::vec3) position);
        bbox.max() = glm::max((glm::vec3) bbox.max(), (glm::vec3) position);
    });
    return bbox;
}

NBody::BoundingBox NBody::Simulation::activeBoundingBox() const {
    BoundingBox bbox;
    auto activeParticles = view<const Physics::Position, const Physics::Mass>();
    activeParticles.each([&](const auto &position, const auto &mass) {
        bbox.min() = glm::min((glm::vec3) bbox.min(), (glm::vec3) position);
        bbox.max() = glm::max((glm::vec3) bbox.max(), (glm::vec3) position);
    });
    return bbox;
}

NBody::BoundingBox NBody::Simulation::passiveBoundingBox() const {
    BoundingBox bbox;
    auto passiveParticles = view<const Physics::Position, const Physics::Acceleration>();
    passiveParticles.each([&](const auto &position, const auto &acceleration) {
        bbox.min() = glm::min((glm::vec3) bbox.min(), (glm::vec3) position);
        bbox.max() = glm::max((glm::vec3) bbox.max(), (glm::vec3) position);
    });
    return bbox;
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

        if (s.all_of<NBody::Physics::Acceleration>(entity))
            e["acceleration"] = s.get<NBody::Physics::Acceleration>(entity);

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

    auto entities = std::vector<Simulation::entity_type>{j["particles"].size()};
    s.create(entities.begin(), entities.end());

    for (int i = 0; i < entities.size(); ++i) {

        auto p = j["particles"][i];
        Simulation::Particle particle = {s, entities[i]};

        if (p.contains("position"))
            particle.setPosition(p["position"].get<NBody::Physics::Position>());

        if (p.contains("velocity"))
            particle.setVelocity(p["velocity"].get<NBody::Physics::Velocity>());

        if (p.contains("mass"))
            particle.setMass(p["mass"].get<float>());

        if (!p.contains("passive") || p["passive"].get<bool>() // todo: remove
            || p.contains("acceleration"))
            particle.emplace<NBody::Physics::Acceleration>(0.0f, 0.0f, 0.0f); // Passive particles have acceleration

        if (p.contains("color"))
            particle.setColor(p["color"].get<NBody::Graphics::Color>());

        if (p.contains("sphere"))
            particle.setSphere(p["sphere"].get<NBody::Graphics::Sphere>());

        // Notify any watchers that this particle has new data
        if (particle.all_of<sigc::signal<void()>>())
            particle.get<sigc::signal<void()>>().emit();
    }

    s.signal_particles_added.emit(entities);
    s.signal_changed.emit();

    spdlog::debug("Read {} particles", j["particles"].size());
}

template<typename T>
T swapEndian(T u) {
    static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

    union {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

template<typename T>
T correctEndian(T u, bool shouldSwap) {
    return shouldSwap ? swapEndian(u) : u;
}

void NBody::from_tipsy(std::ifstream &in, NBody::Simulation &s) {
    // see: https://github.com/N-BodyShop/pytipsy/blob/master/pytipsy.py

    assert(in.good());

    // Determine the actual size of the file
    in.seekg(0, std::ios::end);
    std::size_t fs = in.tellg();
    in.seekg(0, std::ios::beg);

    // Read the header
    double t;
    std::uint32_t n, ndim, ng, nd, ns;
    in.read(reinterpret_cast<char *>(&t), sizeof(t));
    in.read(reinterpret_cast<char *>(&n), sizeof(n));
    in.read(reinterpret_cast<char *>(&ndim), sizeof(ndim));
    in.read(reinterpret_cast<char *>(&ng), sizeof(ng));
    in.read(reinterpret_cast<char *>(&nd), sizeof(nd));
    in.read(reinterpret_cast<char *>(&ns), sizeof(ns));

    // If the number of dimensions is unreasonable, the endianness must be incorrect
    bool wrongEndian = (ndim < 1 || 3 < ndim);
    t = correctEndian(t, wrongEndian);
    n = correctEndian(n, wrongEndian);
    ndim = correctEndian(ndim, wrongEndian);
    ng = correctEndian(ng, wrongEndian);
    nd = correctEndian(nd, wrongEndian);
    ns = correctEndian(ns, wrongEndian);

    spdlog::debug("Tipsy dataset properties:\n"
                  "\tn: {}\n"
                  "\tdimensions: {}\n"
                  "\tGaseous Matter particles: {}\n"
                  "\tDark Matter particles: {}\n"
                  "\tStars: {}",
                  n, ndim, ng, nd, ns);

    // If the file size indicates that padding is present, skip it
    if (fs == 32 + 48 * ng + 36 * nd + 44 * ns)
        in.seekg(4, std::ios::cur);
    else if (fs != 28 + 48 * ng + 36 * nd + 44 * ns) {
        spdlog::error("File size does not match header!");
        return;
    }

    // Read gaseous matter
    // Skip this for now
    in.seekg(48 * ng, std::ios::cur);

    // Read dark matter
    // Skip this for now
    in.seekg(36 * nd, std::ios::cur);

    // Read stars
    auto entities = std::vector<Simulation::entity_type>{ns};
    s.create(entities.begin(), entities.end());
    for (int i = 0; i < entities.size(); ++i) {
        if (!in.good()) spdlog::error("Encountered an issue while reading the file");

        float mass, x, y, z, vx, vy, vz, metals, tform, eps, phi;
        in.read(reinterpret_cast<char *>(&mass), sizeof(mass));
        in.read(reinterpret_cast<char *>(&x), sizeof(x));
        in.read(reinterpret_cast<char *>(&y), sizeof(y));
        in.read(reinterpret_cast<char *>(&z), sizeof(z));
        in.read(reinterpret_cast<char *>(&vx), sizeof(vx));
        in.read(reinterpret_cast<char *>(&vy), sizeof(vy));
        in.read(reinterpret_cast<char *>(&vz), sizeof(vz));
        in.read(reinterpret_cast<char *>(&metals), sizeof(metals));
        in.read(reinterpret_cast<char *>(&tform), sizeof(tform));
        in.read(reinterpret_cast<char *>(&eps), sizeof(eps));
        in.read(reinterpret_cast<char *>(&phi), sizeof(phi));
        mass = correctEndian(mass, wrongEndian);
        x = correctEndian(x, wrongEndian);
        y = correctEndian(y, wrongEndian);
        z = correctEndian(z, wrongEndian);
        vx = correctEndian(vx, wrongEndian);
        vy = correctEndian(vy, wrongEndian);
        vz = correctEndian(vz, wrongEndian);
        metals = correctEndian(metals, wrongEndian);
        tform = correctEndian(tform, wrongEndian);
        eps = correctEndian(eps, wrongEndian);
        phi = correctEndian(phi, wrongEndian);

        // Choose a color based on Velocity
        float magnitude = glm::length(glm::vec3{vx, vy, vz});
        auto red = std::abs(vx) / magnitude;
        auto blue = std::abs(vy) / magnitude;
        auto green = std::abs(vz) / magnitude;
        //auto multiplier = (1.0f - (magnitude / (1.0f - std::pow(magnitude, 2.0f))));
        //        auto red = 1.0f - (vx / (1.0f + std::pow(vx, 2.0f)));
        //        auto green = 1.0f - (vy / (1.0f + std::pow(vy, 2.0f)));
        //        auto blue = 1.0f - (vz / (1.0f + std::pow(vz, 2.0f)));

        Simulation::Particle particle = {s, entities[i]};
        particle.setMass(mass)
                .setPosition({x, y, z})
                .setVelocity({vx, vy, vz})
                .setAcceleration({0.0f, 0.0f, 0.0f})
                .setSphere({std::cbrt(mass)})
                .setColor({red, green, blue});

        // Notify any watchers that this particle has new data
        if (particle.all_of<sigc::signal<void()>>())
            particle.get<sigc::signal<void()>>().emit();


    }

    s.signal_particles_added(entities);
    s.signal_changed.emit();

    spdlog::debug("Read {} particles", entities.size());
}
