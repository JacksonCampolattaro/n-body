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

            PassiveElement(float mass);

            float _mass;

            std::vector<Velocity> *_velocityVector;
            size_t _velocityIndex;

            const std::vector<Position> *_positionVector;
            size_t _positionIndex;
        };

    }
}


#endif //N_BODY_PASSIVEELEMENT_H
