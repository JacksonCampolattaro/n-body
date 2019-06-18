//
// Created by jackcamp on 6/18/19.
//

#ifndef N_BODY_COLOR_H
#define N_BODY_COLOR_H


#include <glm/vec3.hpp>

class Color : public glm::vec3 {

    template<class Archive>
    void serialize(Archive & ar);
};


#endif //N_BODY_COLOR_H
