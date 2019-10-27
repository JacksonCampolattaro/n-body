//
// Created by jackcamp on 6/18/19.
//

#ifndef N_BODY_RGBACOLOR_H
#define N_BODY_RGBACOLOR_H


#include <glm/vec4.hpp>

class rgbaColor : public glm::vec4 {
public:

    rgbaColor(float r, float g, float b, float h) : glm::vec4(r, g, b, h){};
    rgbaColor(glm::vec4 vector) : glm::vec4(vector){};

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(
                cereal::make_nvp("R", this->r), cereal::make_nvp("G", this->g), cereal::make_nvp("B", this->b), cereal::make_nvp("A", this->a)
        );
    }
};


#endif //N_BODY_RGBACOLOR_H
