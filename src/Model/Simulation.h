//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_SIMULATION_H
#define N_BODY_SIMULATION_H

#include "Entity.h"
#include "Position.h"
#include "Velocity.h"
#include "Drawable/Drawable.h"
#include "Physics/Rule.h"
#include "Physics/PassiveElement.h"
#include "Physics/ActiveElement.h"
#include "../Controller/Logger.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

#include <sigc++/signal.h>

#include <vector>
#include <deque>
#include <memory>
#include <string>

namespace Model {

    class Simulation {

    public:

        Simulation() = default;

        Entity &newEntity();

        void update();

        sigc::signal<void, const std::deque<Drawable::Drawable> &> signal_update_complete;
        sigc::signal<void, const std::deque<Drawable::Drawable> &> signal_num_drawables_changed;

        Physics::Rule _rule;

        std::deque<Entity> _entities;
        std::deque<Position> _positions;
        std::deque<Velocity> _velocities;
        std::deque<Physics::PassiveElement> _passiveElements;
        std::deque<Physics::ActiveElement> _activeElements;
        std::deque<Drawable::Drawable> _drawables;
    };

}


#endif //N_BODY_SIMULATION_H
