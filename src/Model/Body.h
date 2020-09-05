//
// Created by jackcamp on 9/4/20.
//

#ifndef N_BODY_BODY_H
#define N_BODY_BODY_H

#include "Handle.h"
#include "Position.h"
#include "Velocity.h"
#include "Drawable/Drawable.h"
#include "Physics/ActiveElement.h"
#include "Physics/PassiveElement.h"

#include <variant>
#include <iostream>

namespace Model {

    class Body {
    public:

        Body();

        friend std::ostream& operator<<(std::ostream& os, const Body &body);

    private:

        friend class Simulation;

        std::variant<Position, Handle<Position>> _position;
        std::variant<Velocity, Handle<Velocity>> _velocity;
        std::variant<Drawable::Drawable, Handle<Drawable::Drawable>, std::monostate> _drawable;
        std::variant<Physics::ActiveElement, Handle<Physics::ActiveElement>, std::monostate> _passiveElement;
        std::variant<Physics::PassiveElement, Handle<Physics::PassiveElement>, std::monostate> _activeElement;

    };

}

#endif //N_BODY_BODY_H
