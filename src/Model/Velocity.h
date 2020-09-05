//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_VELOCITY_H
#define N_BODY_VELOCITY_H

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

#include <glm/vec3.hpp>
#include <iomanip>

namespace Model {

    class Velocity : public glm::vec3 {

    public:

        Velocity(float x, float y, float z) : glm::vec3(x, y, z) {};

        Velocity(glm::vec3 vector) : glm::vec3(vector) {};

        [[nodiscard]] std::string toString() const;

        friend std::ostream &operator<<(std::ostream &os, const Velocity &velocity) {

            os << "{ "
               << "\"velocity\" : "
               << "\""
               << velocity.x << " "
               << velocity.y << " "
               << velocity.z
               << "\""
               << " }";
            return os;
        }

        friend std::istream &operator>>(std::istream &in, Velocity &velocity) {

            std::string _;

            // The first value should be the open bracket
            in >> _;
            assert("{" == _);
            {

                // The next value should be the name
                in >> std::quoted(_);
                assert("velocity" == _);

                // The next value should be the colon
                in >> _;
                assert(":" == _);

                // Get the value
                std::string value;
                in >> std::quoted(value);
                std::stringstream(value) >> velocity.x >> velocity.y >> velocity.z;

            }
            // The last value should be the close bracket
            in >> _;
            assert("}" == _);

            return in;
        }
    };

}


#endif //N_BODY_VELOCITY_H
