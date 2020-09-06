//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_POSITION_H
#define N_BODY_POSITION_H

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include <glm/vec3.hpp>
#include <iomanip>

namespace Model {

    class Position : public glm::vec3 {

    public:

        Position(float x, float y, float z) : glm::vec3(x, y, z) {};

        Position(glm::vec3 vector) : glm::vec3(vector) {};

        [[nodiscard]] std::string toString() const;

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
        }

        bool Deserialize(const rapidjson::Value &obj) {

            x = obj["x"].GetFloat();
            y = obj["y"].GetFloat();
            z = obj["z"].GetFloat();

            return true;
        }

        friend std::ostream &operator<<(std::ostream &os, const Position &position) {
            return os;
        }


        friend std::istream &operator>>(std::istream &in, Position &position) {
            return in;
        }
    };

}

#endif //N_BODY_POSITION_H
