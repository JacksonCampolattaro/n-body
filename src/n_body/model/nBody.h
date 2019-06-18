//
// Created by jackcamp on 6/18/19.
//

#ifndef N_BODY_NBODY_H
#define N_BODY_NBODY_H


#include "../tools/Position.h"
#include "../tools/Velocity.h"
#include "../view/Drawable.h"

class nBody {

public:

private:

    // Used in physics calculations

    Position position = Position(0, 0, 0);
    Position nextPosition = Position(0, 0, 0);

    Velocity velocity = Velocity(0, 0, 0);
    Velocity nextVelocity = Velocity(0, 0, 0);

    float mass = 1;


    // Used for drawing

    Drawable *drawable;

};


#endif //N_BODY_NBODY_H
