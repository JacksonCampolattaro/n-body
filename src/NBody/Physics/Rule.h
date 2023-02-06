//
// Created by jackcamp on 4/12/22.
//

#ifndef N_BODY_RULE_H
#define N_BODY_RULE_H

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>

#include "Position.h"
#include "Acceleration.h"
#include "Force.h"
#include "Mass.h"

namespace NBody::Physics {

    class Rule {
    private:

        float _g;
        float _epsilon;

    public:

        explicit Rule(const float &G = 1.0, const float &epsilon = 0.0001) : _g(G), _epsilon(epsilon) {};

        Force operator()(const Position &activePosition, const Mass &activeMass,
                         const Position &passivePosition, const Mass &passiveMass) const {

            if (activePosition == passivePosition) return {};

            float force = _g * passiveMass.mass() * activeMass.mass() /
                          (glm::distance2((glm::vec3) activePosition, (glm::vec3) passivePosition) + _epsilon);

            return glm::normalize(activePosition - passivePosition) * force;
        }

        Acceleration operator()(const Position &activePosition, const Mass &activeMass,
                                const Position &passivePosition) const {

            if (activePosition == passivePosition) return {};

            float force = _g * activeMass.mass() /
                          (glm::distance2((glm::vec3) activePosition, (glm::vec3) passivePosition) + _epsilon);

            return glm::normalize(activePosition - passivePosition) * force;
        }

        float &g() { return _g; }

        float &epsilon() { return _epsilon; }
    };

}

#endif //N_BODY_RULE_H
