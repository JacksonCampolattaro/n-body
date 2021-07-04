//
// Created by jackcamp on 9/2/20.
//

#ifndef N_BODY_PASSIVEELEMENT_H
#define N_BODY_PASSIVEELEMENT_H

#include <NBody/Physics/Position.h>
#include <NBody/Physics/Velocity.h>

namespace NBody::Simulation::Physics {

    class PassiveElement {
    public:

        explicit PassiveElement(float mass);

        [[nodiscard]] float &mass();
        [[nodiscard]] const float &mass() const;

    private:

        float _mass;
    };

    bool operator==(const PassiveElement &left, const PassiveElement &right);

    void to_json(json &j, const PassiveElement &passiveElement);

    void from_json(const json &j, PassiveElement &passiveElement);

}


#endif //N_BODY_PASSIVEELEMENT_H
