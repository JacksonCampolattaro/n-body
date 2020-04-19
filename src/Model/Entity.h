//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_ENTITY_H
#define N_BODY_ENTITY_H

#include "Position.h"
#include "Velocity.h"
#include "Drawable/Drawable.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>

#include <memory>

namespace Model {

    /**
     * @brief A class tying together the aspects that describe an object.
     *
     * This class provides easy access to the components that make up an object in the simulation.
     * Notably, it contains none of the components directly. For cache optimization reasons,
     * the components themselves are stored in contiguous collections.
     * This class contains smart pointers to its (optional) aspects.
     */
    class Entity {

    public:

        Entity();

        void setPosition(const Position &position);

        void setVelocity(const Velocity &velocity);

        std::string toString();

        template<class Archive>
        void serialize(Archive & ar) {

            Position position = *_position;
            Position velocity = *_velocity;
            ar(
                    cereal::make_nvp("position", position),
                    cereal::make_nvp("velocity", velocity)
            );
            *_position = position;
            *_velocity = velocity;

        }

    private:

        friend class Simulation;

        std::shared_ptr<Position> _position;
        std::shared_ptr<Velocity> _velocity;

        std::shared_ptr<Drawable::Drawable> _drawable;

    };

}


#endif //N_BODY_ENTITY_H
