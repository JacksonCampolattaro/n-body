//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_SIMULATION_H
#define N_BODY_SIMULATION_H

#include "Position.h"
#include "Velocity.h"
#include "Entity.h"
#include "Physics/Rule.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

#include <vector>
#include <memory>

namespace Model {

    class Simulation {

    public:

        /**
         * @brief Adds a new entity to the simulation
         *
         * Aspects of the entity are added to their respective collections.
         * The entity's references to those aspects are reassigned to their new locations.
         *
         * @param entity The entity to be added
         */
        void addEntity(std::shared_ptr<Entity> entity);

        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(
                    cereal::make_nvp("entities", _entities)
            );
        }

    private:

        Physics::Rule _rule;

        std::vector<std::shared_ptr<Entity>> _entities;

        std::vector<Position> _positions;
        std::vector<Velocity> _velocities;
    };

}


#endif //N_BODY_SIMULATION_H
