//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_ENTITY_H
#define N_BODY_ENTITY_H

#include "Position.h"
#include "Velocity.h"
#include "Drawable/Drawable.h"

#include <cereal/cereal.hpp>
#include <memory>

namespace Model {

    /**
     * @brief A class tying together the aspects that describe an object.
     *
     * TODO
     */
    class Entity {

    public:

    private:

        std::shared_ptr<Position> _position;
        std::shared_ptr<Velocity> _velocity;

        std::shared_ptr<Drawable::Drawable> _drawable;

        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(
                    CEREAL_NVP(_position),
                    CEREAL_NVP(_velocity)
            );

        }
    };

}


#endif //N_BODY_ENTITY_H
