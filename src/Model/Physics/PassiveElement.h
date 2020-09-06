//
// Created by jackcamp on 9/2/20.
//

#ifndef N_BODY_PASSIVEELEMENT_H
#define N_BODY_PASSIVEELEMENT_H

#include "../Position.h"
#include "../Velocity.h"

namespace Model {
    namespace Physics {

        class PassiveElement {
        public:

            PassiveElement(float mass);

            const Position &position() const;

            Velocity &velocity();

            float _mass;
            Velocity *_velocity;
            const Position *_position;

            template<typename Writer>
            friend Writer &operator<<(Writer &writer, const PassiveElement &passiveElement) {
                writer.StartObject();
                {
                    writer.String("mass");
                    writer.Double(passiveElement._mass);
                }
                writer.EndObject();
                return writer;
            }

            friend const rapidjson::Value &operator>>(const rapidjson::Value &obj, PassiveElement &passiveElement) {

                passiveElement._mass = obj["mass"].GetFloat();

                return obj;
            }

            friend std::ostream &operator<<(std::ostream &os, const PassiveElement &passiveElement) {
                os << "-> " << passiveElement._mass << " <-";
                return os;
            }

            friend bool operator==(const PassiveElement &left, const PassiveElement &right) {
                // TODO: This could be improved
                return left._mass == right._mass;
            }
        };

    }
}


#endif //N_BODY_PASSIVEELEMENT_H
