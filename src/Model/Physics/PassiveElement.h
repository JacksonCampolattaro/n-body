//
// Created by jackcamp on 9/2/20.
//

#ifndef N_BODY_PASSIVEELEMENT_H
#define N_BODY_PASSIVEELEMENT_H

#include "../Position.h"
#include "../Velocity.h"

#include "../Handle.h"

namespace Model {
    namespace Physics {

        class PassiveElement {
        public:

            PassiveElement(Handle <Position> position, Handle <Velocity> velocity, float mass);

            const Position &position() const;

            Velocity &velocity();

            float _mass;
            Handle <Velocity> _velocity;
            const Handle <Position> _position;
        };

    }
}


#endif //N_BODY_PASSIVEELEMENT_H
