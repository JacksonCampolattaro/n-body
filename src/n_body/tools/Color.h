//
// Created by jackcamp on 6/18/19.
//

#ifndef N_BODY_COLOR_H
#define N_BODY_COLOR_H


#include <glm/vec3.hpp>

class Color : public glm::vec3 {

    Color(float r, float g, float b) : glm::vec3(r, g, b){};
    Color(glm::vec3 vector) : glm::vec3(vector){};

    template<class Archive>
    void serialize(Archive & ar);
};


#endif //N_BODY_COLOR_H
