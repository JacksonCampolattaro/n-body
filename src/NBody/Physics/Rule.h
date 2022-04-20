//
// Created by jackcamp on 4/12/22.
//

#ifndef N_BODY_RULE_H
#define N_BODY_RULE_H

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "Position.h"
#include "ActiveMass.h"
#include "PassiveMass.h"

namespace NBody::Physics {

    typedef glm::vec3 Acceleration;

    class Rule {
    private:

        float _g;
        float _epsilon;

    public:

        explicit Rule(const float &G = 1.0, const float &epsilon = 0.0000001) : _g(G), _epsilon(epsilon) {};

        Acceleration operator()(const Position &activePosition, const ActiveMass &activeMass,
                                const Position &passivePosition, const PassiveMass &passiveMass) const {
            float force = _g * passiveMass.mass() * activeMass.mass() /
                          (glm::length(activePosition - passivePosition) + _epsilon);

            glm::vec3 forceVector = glm::normalize(activePosition - passivePosition) * force;

            return forceVector / passiveMass.mass();
        }

        float &g() { return _g; }

        float &epsilong() { return _epsilon; }
    };

}

#endif //N_BODY_RULE_H
