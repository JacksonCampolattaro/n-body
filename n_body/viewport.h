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

    /**
     * Main loop of the application's graphics
     * Runs until the program is closed
     */
    void graphicsLoop();

    /**
     * Function responsible for drawing everything
     */
    void draw();

    /**
     * Allows for more easily setting the color in OpenGL
     * @param color The color as a vector (rgb)
     */
    static void setColor(vec3 color);

    /**
     * Draws a circle in 3d space
     * @param position The location of the circle
     * @param radius The size of the circle
     */
    static void drawCircle(vec3 position, float radius);

    /**
     * GLFW method for reporting errors
     * @param error The error to be passed
     * @param description The description of the error
     */
    static void handleError(int error, const char *description);

    /**
     * GLFW method for responding to a change in the window's size
     * @param window The window that was resized
     * @param width The new width of the window in pixels
     * @param height The new height of the window in pixels
     */
    static void handleResize(GLFWwindow *window, int width, int height);

private:

    // The window
    GLFWwindow *window;

    // The simulation
    simulationState *theSim;

};


#endif //N_BODY_HEADLESS_VIEWPORT_H
