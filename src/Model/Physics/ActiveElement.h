//
// Created by jackcamp on 9/2/20.
//

#ifndef N_BODY_ACTIVEELEMENT_H
#define N_BODY_ACTIVEELEMENT_H


#include "../Position.h"
#include "../Velocity.h"

namespace Model {
    namespace Physics {

        class ActiveElement {
        public:

            ActiveElement(float mass);

            const Position &position() const;

            float _mass;
            Position *_position;
        };

    }
}


#endif //N_BODY_ACTIVEELEMENT_H
