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

            float _mass;

            const std::vector<Position> *_positionVector;
            size_t _positionIndex;
        };

    }
}


#endif //N_BODY_ACTIVEELEMENT_H
