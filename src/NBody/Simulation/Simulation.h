//
// Created by jackcamp on 12/22/20.
//

#ifndef N_BODY_SIMULATION_H
#define N_BODY_SIMULATION_H

#include <NBody/Physics/Position.h>
#include <NBody/Physics/Velocity.h>
#include <NBody/Physics/ActiveMass.h>
#include <NBody/Graphics/Color.h>
#include <NBody/Graphics/Sphere.h>

#include <nlohmann/json.hpp>

#include <entt/entity/registry.hpp>

#include <iostream>
#include <iomanip>

using nlohmann::json;
using std::istream;
using std::ostream;

namespace NBody {

    typedef entt::entity Entity;

    class Simulation : public entt::basic_registry<Entity> {
    public:

        class Body {
        private:
            const Entity _entity;
            Simulation &_simulation;
        public:
            Body(const Entity &entity, Simulation &simulation) : _entity(entity), _simulation(simulation) {}

            Body &setPosition(const Physics::Position &position);

            Body &setVelocity(const Physics::Velocity &velocity);

            Body &setMass(const Physics::ActiveMass &mass);

            Body &setColor(const Graphics::Color &color);

            Body &setSphere(const Graphics::Sphere &sphere);

        };

        Body createBody();

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

}

#endif //N_BODY_SIMULATION_H
