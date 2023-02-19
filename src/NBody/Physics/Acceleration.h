//
// Created by Jackson Campolattaro on 1/17/23.
//

#ifndef N_BODY_ACCELERATION_H
#define N_BODY_ACCELERATION_H

#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Physics {

    class Acceleration : public glm::vec3 {
    public:

        using glm::vec3::vec;

        Acceleration(glm::vec3 vector) : glm::vec3(vector) {};

        // todo: I shouldn't need to implement these things by hand
        inline Acceleration &operator+=(const Acceleration &right) {
            glm::vec3::operator+=((glm::vec3) right);
            return *this;
        }
        inline Acceleration &operator-=(const Acceleration &right) {
            glm::vec3::operator-=((glm::vec3) right);
            return *this;
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Acceleration, x, y, z)
    };

}

#endif //N_BODY_ACCELERATION_H
