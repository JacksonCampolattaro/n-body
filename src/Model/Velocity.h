//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_VELOCITY_H
#define N_BODY_VELOCITY_H

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

#include <glm/vec3.hpp>

namespace Model {

    class Velocity : public glm::vec3 {

    public:

        Velocity(float x, float y, float z) : glm::vec3(x, y, z) {};

        Velocity(glm::vec3 vector) : glm::vec3(vector) {};

        [[nodiscard]] std::string toString() const;

        friend std::ostream &operator<<(std::ostream &os, const Velocity &velocity) {

            os << "< "
               << velocity.x << ", "
               << velocity.y << ", "
               << velocity.z << " >";

            return os;
        }

        friend std::istream &operator>>(std::istream &in, Velocity &velocity) {

            char _;
            in >> _ >> velocity.x >> _ >> velocity.y >> _ >> velocity.z >> _;
            return in;
        }
    };

}


#endif //N_BODY_VELOCITY_H
