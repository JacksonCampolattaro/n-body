//
// Created by jackcamp on 4/5/19.
//

#ifndef N_BODY_PHYSICSCONTEXT_H
#define N_BODY_PHYSICSCONTEXT_H


#include "Body.h"

class physicsContext {

public:

    /**
     * Applies the gravity on the first body from the second and changes the velocity accordingly
     * @param subject the body being affected
     * @param actorPosition the position of the body creating the force
     * @param actorPosition the mass of the body creating the force
     */
    void applyGravityBetweenBodies(Body *subject, glm::vec3 actorPosition, float actorMass);

private:


};


#endif //N_BODY_PHYSICSCONTEXT_H
