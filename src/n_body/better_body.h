//
// Created by jackcamp on 2/25/19.
//

#ifndef N_BODY_BETTER_BODY_H
#define N_BODY_BETTER_BODY_H


#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "drawable.h"

/**
 * Intended to streamline the functionality of the old body
 * The goal is to retain all the same features, while reducing the number of methods.
 */
class better_body : public drawable {
public:

private:


    glm::vec3 position;
    glm::vec3 velocity;


};


#endif //N_BODY_BETTER_BODY_H
