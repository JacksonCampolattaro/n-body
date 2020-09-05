//
// Created by jackcamp on 9/3/20.
//

#ifndef N_BODY_ENTITY_H
#define N_BODY_ENTITY_H

#include "Position.h"
#include "Velocity.h"
#include "Drawable/Drawable.h"
#include "Physics/PassiveElement.h"
#include "Physics/ActiveElement.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

namespace Model {

    class Simulation;

    class Entity {
    public:

        Entity &addDrawable(Color3 color, float radius);

        Entity &addPassiveElement(float mass);
        Entity &addActiveElement(float mass);

    private:

        Entity(Simulation &simulation, Position &position, Velocity &velocity);

        friend class Simulation;

        Simulation &_simulation;

        Position *_position;
        Velocity *_velocity;

        Drawable::Drawable *_drawable;
        Physics::PassiveElement *_passiveElement;
        Physics::ActiveElement *_activeElement;
    };
}


#endif //N_BODY_ENTITY_H
