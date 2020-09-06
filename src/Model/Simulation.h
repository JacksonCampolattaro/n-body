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

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

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

        bool loadBodiesFromPath(const std::string &path);
        bool saveBodiesToPath(const std::string &path);

        sigc::signal<void, const std::deque<Drawable::Drawable> &> signal_update_complete;
        sigc::signal<void, const std::deque<Drawable::Drawable> &> signal_num_drawables_changed;

        Physics::Rule _rule;

        std::deque<Entity> _entities;
        std::deque<Position> _positions;
        std::deque<Velocity> _velocities;
        std::deque<Physics::PassiveElement> _passiveElements;
        std::deque<Physics::ActiveElement> _activeElements;
        std::deque<Drawable::Drawable> _drawables;

        template<typename Writer>
        friend Writer &operator<<(Writer &writer, const Simulation &simulation) {
            writer.StartArray();
            {
                for (auto entity : simulation._entities) {
                    writer << entity;
                }
            }
            writer.EndArray();
            return writer;
        }

        friend const rapidjson::Value &operator>>(const rapidjson::Value &obj, Simulation &simulation) {

            assert(obj.IsArray());
            for (auto it = obj.Begin(); it != obj.End(); ++it)
                *it >> simulation.newEntity();
            return obj;
        }

        friend std::ostream &operator<<(std::ostream &os, const Simulation &simulation) {

            os << "{\n";
            for (auto entity : simulation._entities) {
                os << "    " << entity << "\n";
            }
            os << "}" << std::endl;

            return os;
        }
    };

}


#endif //N_BODY_SIMULATION_H
