//
// Created by jackcamp on 9/3/20.
//

#ifndef N_BODY_ENTITY_H
#define N_BODY_ENTITY_H

#include "Handle.h"
#include "Position.h"
#include "Velocity.h"
#include "Drawable/Drawable.h"

namespace Model {

    class Simulation;

    class Entity {
    public:

        Model::Entity &addDrawable(Color3 color, float radius);

    private:

        Entity(Simulation &simulation, const Position &position, const Velocity &velocity);

        friend class Simulation;

        Simulation &_simulation;

        Handle<Position> _position;
        Handle<Velocity> _velocity;
        std::optional<Handle<Drawable::Drawable>> _drawable;
    };
}


#endif //N_BODY_ENTITY_H
