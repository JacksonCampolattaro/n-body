//
// Created by jackcamp on 9/2/20.
//

#ifndef N_BODY_ACTIVEELEMENT_H
#define N_BODY_ACTIVEELEMENT_H

#include <NBody/Physics/Position.h>
#include <NBody/Physics/Velocity.h>

namespace NBody::Simulation::Physics {

    class ActiveElement {
    public:

        explicit ActiveElement(float mass);

        [[nodiscard]] float &mass();
        [[nodiscard]] const float &mass() const;

    private:

        float _mass;
    };

    bool operator==(const ActiveElement &left, const ActiveElement &right);

    void to_json(json &j, const ActiveElement &activeElement);

    void from_json(const json &j, ActiveElement &activeElement);

}


#endif //N_BODY_ACTIVEELEMENT_H
