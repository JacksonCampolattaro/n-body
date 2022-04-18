//
// Created by jackcamp on 4/12/22.
//

#ifndef N_BODY_GRAVITY_H
#define N_BODY_GRAVITY_H

#include "Rule.h"

namespace NBody::Physics {

    class Gravity : public Rule {
    private:

        float _g;

    public:

        Gravity(const float &G = 1.0) : _g(G) {};

        std::pair<Position, Force> operator()(
                const Position &activePosition, const ActiveMass &activeMass,
                const Position &passivePosition, const PassiveMass &passiveMass
        );
    };

}


#endif //N_BODY_GRAVITY_H
