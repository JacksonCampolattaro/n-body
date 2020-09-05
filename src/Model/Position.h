//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_POSITION_H
#define N_BODY_POSITION_H

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

#include <glm/vec3.hpp>

namespace Model {

    class Position : public glm::vec3 {

    public:

        Position(float x, float y, float z) : glm::vec3(x, y, z) {};

        Position(glm::vec3 vector) : glm::vec3(vector) {};

        [[nodiscard]] std::string toString() const;

        friend std::ostream &operator<<(std::ostream &os, const Position &position) {

            os << "( "
               << position.x << ", "
               << position.y << ", "
               << position.z << " )";

            return os;
        }


        friend std::istream &operator>>(std::istream &in, Position &position) {

            char _;
            in >> _ >> position.x >> _ >> position.y >> _ >> position.z >> _;
            return in;
        }
    };

}

#endif //N_BODY_POSITION_H
