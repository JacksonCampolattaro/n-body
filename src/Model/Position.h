//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_POSITION_H
#define N_BODY_POSITION_H

#include <glm/vec3.hpp>

namespace Model {

    class Position : public glm::vec3 {

    public:

        Position(float x, float y, float z) : glm::vec3(x, y, z){};
        Position(glm::vec3 vector) : glm::vec3(vector){};
    };

}

#endif //N_BODY_POSITION_H
