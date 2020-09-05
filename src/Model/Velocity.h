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

        template<class Archive>
        void serialize(Archive &ar) {
            ar(
                    cereal::make_nvp("Δx", this->x),
                    cereal::make_nvp("Δy", this->y),
                    cereal::make_nvp("Δz", this->z)
            );
        }
    };

}


#endif //N_BODY_VELOCITY_H
