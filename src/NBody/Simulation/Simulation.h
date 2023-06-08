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
#include <entt/entity/group.hpp>
#include <entt/entity/handle.hpp>

#include <glibmm/interface.h>

#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <fstream>

#include <giomm/listmodel.h>
#include <glibmm/dispatcher.h>
#include <giomm/file.h>
#include <glibmm/init.h>

#include "NBody/Physics/BoundingBox.h"

using nlohmann::json;
using std::istream;
using std::ostream;

namespace NBody {

    //typedef entt::entity Entity;
    using Entity = std::uint64_t;

    class Simulation : public entt::basic_registry<Entity> {
    public:

        class Particle : public entt::basic_handle<Simulation>, public Glib::Object {
        public:

            Particle(const Particle &other) : entt::basic_handle<Simulation>(other),
                                              Glib::ObjectBase(typeid(Particle)),
                                              Glib::Object() {}

            Particle(Simulation &ref, Entity value) : entt::basic_handle<Simulation>(ref, value) {}

            Particle &setPosition(const Physics::Position &position);

            Particle &setVelocity(const Physics::Velocity &velocity);

            Particle &setMass(const float &mass);

            Particle &setAcceleration(const Physics::Acceleration &acceleration);

            Particle &setColor(const Graphics::Color &color);

            Particle &setSphere(const Graphics::Sphere &sphere);
        };

        Particle newParticle();

    public:

        Simulation() : entt::basic_registry<Entity>() {
            // Necessary for signals to work properly, should be idempotent
            Glib::init();
        }

        Simulation(const Simulation &other) : entt::basic_registry<Entity>() {

            // Add all entities from the other simulation, retaining IDs
            assign(other.data(), other.data() + other.size(), other.released());

            // Copy components from the other registry
            invokeForEachType([&]<typename T>() {
                // todo: this might break for tag types
                auto view = other.view<T>();
                insert<T>(view.begin(), view.end(), view.storage().rbegin());
            });

        }

        Simulation &operator=(Simulation other) {
            std::swap(*this, other);
            return *this;
        }

        void save(Gio::File &destination) const;

        void load(Gio::File &source);

        std::vector<NBody::Entity> validEntities() const;

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

        template<typename Lambda>
        static void invokeForEachType(Lambda &&lambda) {

            lambda.template operator()<Physics::Position>();
            lambda.template operator()<Physics::Velocity>();
            lambda.template operator()<Physics::Acceleration>();
            lambda.template operator()<Physics::Mass>();

            lambda.template operator()<Graphics::Color>();
            lambda.template operator()<Graphics::Sphere>();
        }

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

        friend inline bool operator==(const Simulation &lhs, const Simulation &rhs) {
            bool equal = true;

            // The two simulations must have the same number of each component
            invokeForEachType([&]<typename T>() {
                if (lhs.storage<T>().size() != rhs.storage<T>().size()) equal = false;
            });
            if (!equal) return false;

            // The two simulations must have the same entity IDs & the same values for each ID
            lhs.each([&](auto e) {
                if (!rhs.valid(e))
                    equal = false;
                else
                    invokeForEachType([&]<typename T>() {

                        if (lhs.all_of<T>(e) != rhs.all_of<T>(e))
                            equal = false;
                        else if (lhs.all_of<T>(e) && lhs.get<const T>(e) != rhs.get<const T>(e))
                            equal = false;

                    });
            });
            return equal;
        }

        friend inline bool operator!=(const Simulation &lhs, const Simulation &rhs) { return !(lhs == rhs); }
    };

    // todo: these should be declared elsewhere
    using ActiveView = decltype(std::declval<Simulation>().view<const Position, const Physics::Mass>());
    using PassiveView = decltype(std::declval<Simulation>().view<const Position, Physics::Acceleration>());

    void to_json(json &j, const Simulation &s);

    void from_json(const json &j, Simulation &s);

    void from_tipsy(std::ifstream &in, Simulation &s);

}

#endif //N_BODY_SIMULATION_H
