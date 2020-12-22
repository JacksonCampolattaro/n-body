//
// Created by jackcamp on 12/22/20.
//

#ifndef N_BODY_SIMULATION_H
#define N_BODY_SIMULATION_H

#include <entt/entity/registry.hpp>

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Simulation {

    class Simulation {
    public:

        friend void to_json(json& j, const Simulation& s) {

        }

        friend void from_json(const json& j, Simulation& s) {

        }

    private:

        entt::registry _registry;
    };

}

#endif //N_BODY_SIMULATION_H
