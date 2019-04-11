//
// Created by jackcamp on 4/11/19.
//

#ifndef N_BODY_GLTOOLKIT_H
#define N_BODY_GLTOOLKIT_H


#include <GLFW/glfw3.h> // GLFW makes OpenGL simpler to use
#include <glm/glm.hpp> // Include all of glm, because I'm handling many different types of vectors

class GLToolkit {

public:

    /**
     * Allows for setting the color in OpenGL from outside the viewport
     * @param color The color as a vector (rgb)
     */
    static void setColor(glm::vec3 color);

    /**
     * Draws a circle in 3d space
     * @param position The location of the circle
     * @param radius The size of the circle
     */
    static void drawCircle(glm::vec3 position, float radius);

    /**
     * Draws a circle in 3d space
     * @param position The location of the circle
     * @param radius The size of the circle
     */
    static void drawSphere(glm::vec3 position, float radius);
};


#endif //N_BODY_GLTOOLKIT_H
