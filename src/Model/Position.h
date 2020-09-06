//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_POSITION_H
#define N_BODY_POSITION_H

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <glm/vec3.hpp>
#include <iomanip>

namespace Model {

    class Position : public glm::vec3 {

    public:

        Position(float x, float y, float z) : glm::vec3(x, y, z) {};

        Position(glm::vec3 vector) : glm::vec3(vector) {};

        template<typename Writer>
        friend Writer &operator<<(Writer &writer, const Position &position) {
            writer.StartObject();
            {
                writer.String("x");
                writer.Double(position.x);
                writer.String("y");
                writer.Double(position.y);
                writer.String("z");
                writer.Double(position.z);
            }
            writer.EndObject();
            return writer;
        }

        friend const rapidjson::Value &operator>>(const rapidjson::Value &obj, Position &position) {

            position.x = obj["x"].GetFloat();
            position.y = obj["y"].GetFloat();
            position.z = obj["z"].GetFloat();

            return obj;
        }

        friend std::ostream &operator<<(std::ostream &os, const Position &position) {
            os << "( " << position.x << " " << position.y << " " << position.z << " )";
            return os;
        }
    };

}

#endif //N_BODY_POSITION_H
