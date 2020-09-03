//
// Created by jackcamp on 9/3/20.
//

#ifndef N_BODY_ENTITY_H
#define N_BODY_ENTITY_H

#include "Handle.h"
#include "Position.h"
#include "Velocity.h"

namespace Model {

    class Simulation;

    class Entity {
    public:

    private:

        friend class Simulation;

        Entity(Simulation &simulation, const Position &position, const Velocity &velocity);

        Simulation &_simulation;
        Handle<Position> _position;
        Handle<Velocity> _velocity;
    };
}


#endif //N_BODY_ENTITY_H
