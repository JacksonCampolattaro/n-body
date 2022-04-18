//
// Created by jackcamp on 4/12/22.
//

#include "Gravity.h"

namespace NBody::Physics {

    std::pair<Position, Force> NBody::Physics::Gravity::operator()(const NBody::Physics::Position &activePosition,
                                                                   const NBody::Physics::ActiveMass &activeMass,
                                                                   const NBody::Physics::Position &passivePosition,
                                                                   const NBody::Physics::PassiveMass &passiveMass) {
        return {};
    }
}
