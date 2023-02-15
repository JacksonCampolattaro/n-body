//
// Created by Jackson Campolattaro on 2/15/23.
//

#ifndef N_BODY_QUADRUPOLEACCELERATION_H
#define N_BODY_QUADRUPOLEACCELERATION_H

#include <NBody/Physics/Acceleration.h>
#include <NBody/Physics/Quadrupole.h>

namespace NBody::Physics {

    class QuadrupoleAcceleration : public Acceleration, public Quadrupole {
    public:

        QuadrupoleAcceleration() = default;

    };
}

#endif //N_BODY_QUADRUPOLEACCELERATION_H
