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
        float _epsilon = 0.0000001;

    public:

        Gravity(const float &G = 1.0) : _g(G) {};

        Acceleration operator()(
                const Position &activePosition, const ActiveMass &activeMass,
                const Position &passivePosition, const PassiveMass &passiveMass
        ) override;
    };

}


#endif //N_BODY_GRAVITY_H
