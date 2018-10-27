//
// Created by jackcamp on 10/25/18.
//

#ifndef N_BODY_HEADLESS_VIEWPORT_H
#define N_BODY_HEADLESS_VIEWPORT_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include <cstdlib>
#include <stdio.h>
#include <iostream>

#include "simulationState.h"


class viewport {

public:

    /**
     * Constructor for the window
     * @param width the width of the window in pixels
     * @param height the height of the window in pixels
     * @param title the status bar title of the window
     */
    explicit viewport(simulationState *theSim, int width = 1000, int height = 1000,
                      const char *title = "n_Body Simulator 0.2.0");

    void graphicsLoop();

    void draw();

    static void drawCircle(vec3 position, float radius);

    static void handleError(int error, const char *description);

    static void handleResize(GLFWwindow *window, int width, int height);

private:

    // The window
    GLFWwindow *window;

    // The simulation
    simulationState *theSim;

};


#endif //N_BODY_HEADLESS_VIEWPORT_H
