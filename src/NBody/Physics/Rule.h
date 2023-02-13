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
#include "Quadrupole.h"

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

            // See: https://github.com/hannorein/rebound/blob/9fb9ee9aa20c547e1e6c67e7a58f07fd7176c181/src/gravity.c
            glm::vec3 differenceInPositions = activePosition - passivePosition;
            float r2 = glm::length2(differenceInPositions);
            float r = std::sqrt(r2 + _epsilon);
            float scaling = -_g * activeMass.mass() / (r * r * r);
            return scaling * differenceInPositions;
        }

        Acceleration operator()(const Position &activePosition, const Mass &activeMass, const Quadrupole &moment,
                                const Position &passivePosition) const {

            if (activePosition == passivePosition) return {};

            // See: https://github.com/hannorein/rebound/blob/9fb9ee9aa20c547e1e6c67e7a58f07fd7176c181/src/gravity.c
            glm::vec3 d = activePosition - passivePosition;
            float r2 = glm::length2(d);
            float r = std::sqrt(r2 + _epsilon);
            float scaling = -_g * activeMass.mass() / (r * r * r);

            float qScaling = _g / (r * r * r * r * r);

            float mrr = d.x * d.x * moment.xx() + d.y * d.y * moment.yy() + d.z * d.z * moment.zz() +
                        2.0f * (d.x * d.y * moment.xy() + d.x * d.z * moment.xz() + d.y * d.z * moment.yz());
            float combinedScaling = scaling + (qScaling * -5.0f / (2.0f * r * r) * mrr);

            return glm::vec3{
                    qScaling * (d.x * moment.xx() + d.y * moment.xy() + d.z * moment.xz()),
                    qScaling * (d.x * moment.xy() + d.y * moment.yy() + d.z * moment.yz()),
                    qScaling * (d.x * moment.xz() + d.y * moment.yz() + d.z * moment.zz())
            } + (d * combinedScaling);

        }

        float &g() { return _g; }

        float &epsilon() { return _epsilon; }
    };

}

#endif //N_BODY_RULE_H
