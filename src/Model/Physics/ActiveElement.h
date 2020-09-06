//
// Created by jackcamp on 9/2/20.
//

#ifndef N_BODY_ACTIVEELEMENT_H
#define N_BODY_ACTIVEELEMENT_H

#include "../Position.h"
#include "../Velocity.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

namespace Model {
    namespace Physics {

        class ActiveElement {
        public:

            ActiveElement(float mass);

            const Position &position() const;

            float _mass;
            Position *_position;

            template<typename Writer>
            friend Writer &operator<<(Writer &writer, const ActiveElement &activeElement) {
                writer.StartObject();
                {
                    writer.String("mass");
                    writer.Double(activeElement._mass);
                }
                writer.EndObject();
            }

            friend const rapidjson::Value &operator>>(const rapidjson::Value &obj, ActiveElement &activeElement) {

                activeElement._mass = obj["mass"].GetFloat();

                return obj;
            }

            friend std::ostream &operator<<(std::ostream &os, const ActiveElement &activeElement) {
                os << "<- " << activeElement._mass << " ->";
                return os;
            }

            friend bool operator==(const ActiveElement &left, const ActiveElement &right) {
                // TODO: This could be improved
                return left._mass == right._mass;
            }
        };

    }
}


#endif //N_BODY_ACTIVEELEMENT_H
