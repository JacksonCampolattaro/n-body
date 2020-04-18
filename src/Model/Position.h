//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_POSITION_H
#define N_BODY_POSITION_H

#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>

#include <glm/vec3.hpp>

namespace Model {

    class Position : public glm::vec3 {

    public:

        Position(float x, float y, float z) : glm::vec3(x, y, z){};
        Position(glm::vec3 vector) : glm::vec3(vector){};

        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(
                    cereal::make_nvp("x", this->x), cereal::make_nvp("y", this->y), cereal::make_nvp("z", this->z)
            );
        }
    };

}

#endif //N_BODY_POSITION_H
