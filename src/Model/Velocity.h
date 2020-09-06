//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_VELOCITY_H
#define N_BODY_VELOCITY_H

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <glm/vec3.hpp>
#include <iomanip>

namespace Model {

    class Velocity : public glm::vec3 {

    public:

        Velocity(float x, float y, float z) : glm::vec3(x, y, z) {};

        Velocity(glm::vec3 vector) : glm::vec3(vector) {};

        template<typename Writer>
        friend Writer &operator<<(Writer &writer, const Velocity &velocity) {
            writer.StartObject();
            {
                writer.String("dx");
                writer.Double(velocity.x);
                writer.String("dy");
                writer.Double(velocity.y);
                writer.String("dz");
                writer.Double(velocity.z);
            }
            writer.EndObject();
            return writer;
        }

        friend const rapidjson::Value &operator>>(const rapidjson::Value &obj, Velocity &velocity) {

            velocity.x = obj["dx"].GetFloat();
            velocity.y = obj["dy"].GetFloat();
            velocity.z = obj["dz"].GetFloat();

            return obj;
        }

        friend std::ostream &operator<<(std::ostream &os, const Velocity &velocity) {

            os << "< " << velocity.x << " " << velocity.y << " " << velocity.z << " >";
            return os;
        }

    };

}


#endif //N_BODY_VELOCITY_H
