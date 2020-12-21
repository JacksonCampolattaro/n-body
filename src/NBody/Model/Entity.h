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

                if (entity._drawable) {
                    writer.String("drawable");
                    writer << *entity._drawable;
                }

                if (entity._passiveElement) {
                    writer.String("passiveElement");
                    writer << *entity._passiveElement;
                }

                if (entity._activeElement) {
                    writer.String("activeElement");
                    writer << *entity._activeElement;

                }
            }
            writer.EndObject();
            return writer;
        }

        friend const rapidjson::Value &operator>>(const rapidjson::Value &obj, Entity &entity) {

            Position position(0, 0, 0);
            obj["position"] >> position;
            entity.setPosition(position);

            Velocity velocity(0, 0, 0);
            obj["velocity"] >> velocity;
            entity.setVelocity(velocity);

            if (obj.HasMember("drawable")) {
                Drawable::Drawable drawable{{0, 0, 0}, 0};
                obj["drawable"] >> drawable;
                entity.setDrawable(drawable);
            }

            if (obj.HasMember("passiveElement")) {
                Physics::PassiveElement passiveElement{0};
                obj["passiveElement"] >> passiveElement;
                entity.setPassiveElement(passiveElement);
            }

            if (obj.HasMember("activeElement")) {
                Physics::ActiveElement activeElement{0};
                obj["activeElement"] >> activeElement;
                entity.setActiveElement(activeElement);
            }

            return obj;
        }

        friend std::ostream &operator<<(std::ostream &os, const Entity &entity) {

            os << "{ "
               << *entity._position << " "
               << *entity._velocity << " ";
            if (entity._drawable) { os << *entity._drawable << " "; }
            if (entity._passiveElement) { os << *entity._passiveElement << " "; }
            if (entity._activeElement) { os << *entity._activeElement << " "; }
            os << "}";

            return os;
        }

        friend bool operator==(const Entity &left, const Entity &right) {

            return (
                    left._position == right._position &&
                    left._velocity == right._velocity &&
                    left._drawable == right._drawable &&
                    left._passiveElement == right._passiveElement &&
                    left._activeElement == right._activeElement
            );
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
