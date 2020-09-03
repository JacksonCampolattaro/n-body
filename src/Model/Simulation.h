//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_SIMULATION_H
#define N_BODY_SIMULATION_H

#include "Entity.h"
#include "Position.h"
#include "Velocity.h"
#include "Drawable/Drawable.h"
#include "Physics/Rule.h"
#include "Physics/PassiveElement.h"
#include "Physics/ActiveElement.h"
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

        void addEntity(Position position, Velocity velocity, const Drawable::Drawable &drawable);

        void addEntity(Position position, Velocity velocity, const Drawable::Drawable &drawable,
                       float mass);

        void update();

        Physics::Rule _rule;

        std::vector<Entity> _entities;
        std::vector<Position> _positions;
        std::vector<Velocity> _velocities;
        std::vector<Physics::PassiveElement> _passiveElements;
        std::vector<Physics::ActiveElement> _activeElements;
        std::vector<std::shared_ptr<Drawable::Drawable>> _drawables;
    };

}


#endif //N_BODY_SIMULATION_H
