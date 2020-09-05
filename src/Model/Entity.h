//
// Created by jackcamp on 9/3/20.
//

#ifndef N_BODY_ENTITY_H
#define N_BODY_ENTITY_H

#include "Handle.h"
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

        Entity(Simulation &simulation, const Position &position, const Velocity &velocity);

        friend class Simulation;

        Simulation &_simulation;

        Handle<Position> _position;
        Handle<Velocity> _velocity;

        std::optional<Handle<Drawable::Drawable>> _drawable;
        std::optional<Handle<Physics::PassiveElement>> _passiveElement;
        std::optional<Handle<Physics::ActiveElement>> _activeElement;
    };
}


#endif //N_BODY_ENTITY_H
