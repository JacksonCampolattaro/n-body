//
// Created by jackcamp on 12/22/20.
//

#ifndef N_BODY_ENTITYCOLLECTION_H
#define N_BODY_ENTITYCOLLECTION_H

#include <nlohmann/json.hpp>
#include <entt/entity/registry.hpp>

using nlohmann::json;

namespace NBody::Simulation {

    typedef entt::entity Entity;

    class EntityCollection : public entt::registry {};

    void to_json(json &j, const EntityCollection &entityCollection);

    void from_json(const json &j, EntityCollection &entityCollection);
}

#endif //N_BODY_ENTITYCOLLECTION_H
