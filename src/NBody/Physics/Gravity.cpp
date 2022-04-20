//
// Created by jackcamp on 4/12/22.
//

#include <glm/geometric.hpp>
#include "Gravity.h"

namespace NBody::Physics {

    Acceleration NBody::Physics::Gravity::operator()(const NBody::Physics::Position &activePosition,
                                                     const NBody::Physics::ActiveMass &activeMass,
                                                     const NBody::Physics::Position &passivePosition,
                                                     const NBody::Physics::PassiveMass &passiveMass) {

        float force = _g * passiveMass.mass() * activeMass.mass() /
                      (glm::length(activePosition - passivePosition) + _epsilon);

        glm::vec3 forceVector = glm::normalize(activePosition - passivePosition) * force;

        return forceVector / passiveMass.mass();
    }
}
