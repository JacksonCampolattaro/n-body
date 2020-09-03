//
// Created by jackcamp on 9/3/20.
//

#ifndef N_BODY_ENTITY_H
#define N_BODY_ENTITY_H

#include "Handle.h"
#include "Position.h"
#include "Velocity.h"

namespace Model {

    class Entity {

    public:

    private:

        Handle<Position> _position;
        Handle<Velocity> _velocity;

    };
}


#endif //N_BODY_ENTITY_H
