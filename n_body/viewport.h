//
// Created by jackcamp on 10/25/18.
//

#ifndef N_BODY_HEADLESS_VIEWPORT_H
#define N_BODY_HEADLESS_VIEWPORT_H

#include <GLFW/glfw3.h>
#include <cstdlib>


class viewport {

public:

    viewport() {

        // Defining the window
        GLFWwindow *window;

        // Initializing GLFW
        if (!glfwInit()) {
            // Exits if GLFW Fails to load
            exit(EXIT_FAILURE);
        }

        // Creating the window
        window = glfwCreateWindow(640, 480, "Simple example", nullptr, nullptr);
        if (!window) {
            // Kills the program if the window fails to load
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // Sets the window as the current OpenGL context
        glfwMakeContextCurrent(window);

        // Sets the number of frames between buffer swaps
        glfwSwapInterval(1);


    }

};


#endif //N_BODY_HEADLESS_VIEWPORT_H
