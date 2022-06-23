//
// Created by jackcamp on 12/22/20.
//

#ifndef N_BODY_SIMULATION_H
#define N_BODY_SIMULATION_H

#include <NBody/Physics/Position.h>
#include <NBody/Physics/Velocity.h>
#include <NBody/Physics/ActiveMass.h>
#include <NBody/Physics/PassiveMass.h>
#include <NBody/Graphics/Color.h>
#include <NBody/Graphics/Sphere.h>

#include <nlohmann/json.hpp>

#include <entt/entity/registry.hpp>
#include <entt/entity/handle.hpp>

#include <glibmm/interface.h>

#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <giomm/listmodel.h>

using nlohmann::json;
using std::istream;
using std::ostream;

namespace NBody {

    typedef entt::entity Entity;

    class Simulation : public entt::basic_registry<Entity> {
    public:

        class Particle : public entt::basic_handle<Entity>, public Glib::Object {
        public:

            //Particle() : entt::basic_handle<Entity>() {}

            Particle(const Particle &other) : entt::basic_handle<Entity>(other),
                                              Glib::ObjectBase(typeid(Particle)),
                                              Glib::Object() {}

            Particle(Simulation &ref, Entity value) : entt::basic_handle<Entity>(ref, value) {}

            Particle &setPosition(const Physics::Position &position);

            Particle &setVelocity(const Physics::Velocity &velocity);

            Particle &setMass(const float &mass);

            Particle &setActiveMass(const Physics::ActiveMass &mass);

            Particle &setPassiveMass(const Physics::PassiveMass &mass);

            Particle &setColor(const Graphics::Color &color);

            Particle &setSphere(const Graphics::Sphere &sphere);
        };

        Particle newParticle();

    public:

        mutable std::mutex mutex;

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

}

#endif //N_BODY_SIMULATION_H
