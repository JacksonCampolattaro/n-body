//
// Created by jackcamp on 9/2/20.
//

#ifndef N_BODY_PASSIVEELEMENT_H
#define N_BODY_PASSIVEELEMENT_H

#include "../Position.h"
#include "../Velocity.h"

namespace Model {
    namespace Physics {

        class PassiveElement {
        public:

            PassiveElement(Position *position, Velocity *velocity, float mass);

            const Position &position() const;

            Velocity &velocity();

            float _mass;
            Velocity *_velocity;
            const Position *_position;
        };

    }
}


#endif //N_BODY_PASSIVEELEMENT_H
