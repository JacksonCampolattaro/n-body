//
// Created by Jackson Campolattaro on 1/17/23.
//

#ifndef N_BODY_ACCELERATION_H
#define N_BODY_ACCELERATION_H

#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include <fmt/format.h>

using nlohmann::json;

namespace NBody::Physics {

    class Acceleration : public glm::vec3 {
    public:

        using glm::vec3::vec;

        Acceleration(glm::vec3 vector) : glm::vec3(vector) {};

        // todo: this is a little bit of a hack
        inline Acceleration at(const Position &p) const { return *this; }

        // todo: I shouldn't need to implement these things by hand
        inline Acceleration &operator+=(const Acceleration &right) {
            glm::vec3::operator+=((glm::vec3) right);
            return *this;
        }

        inline Acceleration &operator-=(const Acceleration &right) {
            glm::vec3::operator-=((glm::vec3) right);
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &out, const Acceleration &v) {
            return out << fmt::format("<{}, {}, {}>", v.x, v.y, v.z);
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Acceleration, x, y, z)
    };

}

#endif //N_BODY_ACCELERATION_H
