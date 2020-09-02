//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_SIMULATION_H
#define N_BODY_SIMULATION_H

#include "Position.h"
#include "Velocity.h"
#include "Entity.h"
#include "Physics/Rule.h"
#include "../Controller/Logger.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

#include <vector>
#include <memory>
#include <string>

namespace Model {

    class Simulation {

    public:

        Simulation() = default;

        Entity &addEntity(Position position, Velocity velocity);
        Entity &addEntity(Position position, Velocity velocity, const Drawable::Drawable &drawable);

        void update();

        Physics::Rule _rule;

        std::vector<Entity> _entities;

        std::vector<Position> _positions;
        std::vector<Velocity> _velocities;
        std::vector<std::shared_ptr<Drawable::Drawable>> _drawables;
    };

}


#endif //N_BODY_SIMULATION_H
