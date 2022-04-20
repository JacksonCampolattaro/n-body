//
// Created by jackcamp on 4/12/22.
//

#ifndef N_BODY_RULE_H
#define N_BODY_RULE_H

#include <glm/vec3.hpp>

#include "Position.h"
#include "ActiveMass.h"
#include "PassiveMass.h"

namespace NBody::Physics {

    typedef glm::vec3 Acceleration;

    class Rule {
    public:

        virtual Acceleration operator()(
                const Position &activePosition, const ActiveMass &activeMass,
                const Position &passivePosition, const PassiveMass &passiveMass
        ) = 0;

    };

}

#endif //N_BODY_RULE_H
