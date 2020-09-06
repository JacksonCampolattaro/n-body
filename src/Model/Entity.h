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

namespace Model {

    class Simulation;

    class Entity {
    public:

        Entity &setPosition(const Position &position);

        Entity &setVelocity(const Velocity &velocity);

        Entity &setDrawable(const Drawable::Drawable &drawable);

        Entity &setPassiveElement(const Physics::PassiveElement &passiveElement);

        Entity &setActiveElement(const Physics::ActiveElement &activeElement);

        template<typename Writer>
        friend Writer &operator<<(Writer &writer, const Entity &entity) {
            writer.StartObject();
            {
                writer.String("position");
                writer << *entity._position;

                writer.String("velocity");
                writer << *entity._velocity;
            }
            writer.EndObject();
        }

        friend const rapidjson::Value &operator>>(const rapidjson::Value &obj, Entity &entity) {

            Position position(0, 0, 0);
            obj >> position;
            entity.setPosition(position);

            Velocity velocity(0, 0, 0);
            obj >> velocity;
            entity.setVelocity(velocity);

            return obj;
        }

        friend std::ostream &operator<<(std::ostream &os, const Entity &entity) {
            os << "{ " << *entity._position << " " << *entity._velocity << " }";
            return os;
        }

        friend bool operator==(const Entity &left, const Entity &right) {
            return *left._position == *right._position && *left._velocity == *right._velocity;
        }
    private:

        Entity(Simulation &simulation);

        friend class Simulation;

        Simulation &_simulation;

        Position *_position;
        Velocity *_velocity;

        Drawable::Drawable *_drawable;
        Physics::PassiveElement *_passiveElement;
        Physics::ActiveElement *_activeElement;

    };
};


#endif //N_BODY_ENTITY_H
