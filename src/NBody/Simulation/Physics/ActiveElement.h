//
// Created by jackcamp on 9/2/20.
//

#ifndef N_BODY_ACTIVEELEMENT_H
#define N_BODY_ACTIVEELEMENT_H

#include <NBody/Simulation/Position.h>
#include <NBody/Simulation/Velocity.h>

namespace NBody::Simulation::Physics {

    class ActiveElement {
    public:

        explicit ActiveElement(float mass);

        [[nodiscard]] float &mass();
        [[nodiscard]] const float &mass() const;

        friend bool operator==(const ActiveElement &left, const ActiveElement &right);

    private:

        float _mass;
    };

    void to_json(json &j, const ActiveElement &activeElement);

    void from_json(const json &j, ActiveElement &activeElement);

}


#endif //N_BODY_ACTIVEELEMENT_H
