//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_POSITION_H
#define N_BODY_POSITION_H

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

#include <glm/vec3.hpp>
#include <iomanip>

namespace Model {

    class Position : public glm::vec3 {

    public:

        Position(float x, float y, float z) : glm::vec3(x, y, z) {};

        Position(glm::vec3 vector) : glm::vec3(vector) {};

        [[nodiscard]] std::string toString() const;

        friend std::ostream &operator<<(std::ostream &os, const Position &position) {

            os << "{ "
               << "\"position\" : "
               << "\""
               << position.x << " "
               << position.y << " "
               << position.z
               << "\""
               << " }";
            return os;
        }


        friend std::istream &operator>>(std::istream &in, Position &position) {

            std::string _;

            // The first value should be the open bracket
            in >> _;
            assert("{" == _);
            {

                // The next value should be the name
                in >> std::quoted(_);
                assert("position" == _);

                // The next value should be the colon
                in >> std::quoted(_);
                assert(":" == _);

                // Get the value
                std::string value;
                in >> std::quoted(value);
                std::stringstream(value) >> position.x >> position.y >> position.z;

            }
            // The last value should be the close bracket
            in >> _;
            assert("}" == _);

//            char _;
//            in >> _ >> position.x >> _ >> position.y >> _ >> position.z >> _;
            return in;
        }
    };

}

#endif //N_BODY_POSITION_H
