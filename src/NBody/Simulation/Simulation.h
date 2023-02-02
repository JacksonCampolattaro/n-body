//
// Created by jackcamp on 12/22/20.
//

#ifndef N_BODY_SIMULATION_H
#define N_BODY_SIMULATION_H

#include <NBody/Physics/Position.h>
#include <NBody/Physics/Velocity.h>
#include <NBody/Physics/Acceleration.h>
#include <NBody/Physics/Mass.h>
#include <NBody/Graphics/Color.h>
#include <NBody/Graphics/Sphere.h>

#include <nlohmann/json.hpp>

#include <entt/entity/registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/group.hpp>

#include <glibmm/interface.h>

#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <fstream>

#include <giomm/listmodel.h>
#include <glibmm/dispatcher.h>
#include <giomm/file.h>

#include "BoundingBox.h"

using nlohmann::json;
using std::istream;
using std::ostream;

namespace NBody {

    typedef entt::entity Entity;

    class Simulation : public entt::basic_registry<Entity> {
    public:

        class Particle : public entt::basic_handle<Entity>, public Glib::Object {
        public:

            Particle(const Particle &other) : entt::basic_handle<Entity>(other),
                                              Glib::ObjectBase(typeid(Particle)),
                                              Glib::Object() {}

            Particle(Simulation &ref, Entity value) : entt::basic_handle<Entity>(ref, value) {}

            Particle &setPosition(const Physics::Position &position);

            Particle &setVelocity(const Physics::Velocity &velocity);

            Particle &setMass(const float &mass);

            Particle &setAcceleration(const Physics::Acceleration &acceleration);

            Particle &setColor(const Graphics::Color &color);

            Particle &setSphere(const Graphics::Sphere &sphere);
        };

        Particle newParticle();

    public:

        Simulation() : entt::basic_registry<Entity>() {}

        void save(Gio::File &destination) const;

        void load(Gio::File &source);

        std::vector<NBody::Entity> validEntities();

        void removeParticle(NBody::Entity e);

    public:

        std::size_t particleCount() const;

        Physics::Position averagePosition() const;

        float totalMass() const;

        Physics::Position centerOfMass() const;

        std::size_t interactionCount() const;

        BoundingBox boundingBox() const;

        BoundingBox activeBoundingBox() const;

        BoundingBox passiveBoundingBox() const;

    public:

        mutable sigc::signal<void()> signal_changed;
        sigc::signal<void(const std::vector<NBody::Entity> &)> signal_particles_added;
        sigc::signal<void(const std::vector<NBody::Entity> &)> signal_particles_removed;

        mutable std::mutex mutex;

    public:

        friend void to_json(json &j, const Simulation &s);

        friend void from_json(const json &j, Simulation &s);

        friend ostream &operator<<(ostream &out, const Simulation &s) {

            json j;
            to_json(j, s);
            out << std::setw(4) << j;
            return out;
        }

        friend istream &operator>>(istream &in, Simulation &s) {

            json j;
            in >> j;
            from_json(j, s);
            return in;
        }
    };

    void to_json(json &j, const Simulation &s);

    void from_json(const json &j, Simulation &s);

    void from_tipsy(std::ifstream &in, Simulation &s);

}

#endif //N_BODY_SIMULATION_H
