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
     * TODO
     */
    class Entity {

    public:

        Entity() = default;

        Entity* setPosition(Position position);

        Entity* setVelocity(Velocity velocity);

        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(
                    cereal::make_nvp("position", _position),
                    cereal::make_nvp("velocity", _velocity)
            );

        }

    private:

        friend class Simulation;

        std::shared_ptr<Position> _position;
        std::shared_ptr<Velocity> _velocity;

        std::shared_ptr<Drawable::Drawable> _drawable;

    };

}


#endif //N_BODY_ENTITY_H
