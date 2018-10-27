//
// Created by jackcamp on 10/25/18.
//

#ifndef N_BODY_HEADLESS_VIEWPORT_H
#define N_BODY_HEADLESS_VIEWPORT_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

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
                      const char *title = "n_Body Simulator 0.2.0") {

        // Setting the simulation
        this->theSim = theSim;

        // Setting the function for handling errors
        glfwSetErrorCallback(handleError);

        // Initializing GLFW
        if (!glfwInit()) {
            // Exits if GLFW Fails to load
            exit(EXIT_FAILURE);
        }

        // Creating the window
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window) {
            // Kills the program if the window fails to load
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // Setting the function for handling window resizing
        glfwSetWindowSizeCallback(window, handleResize);

        // Setting the window's minimum size
        glfwSetWindowSizeLimits(window, 400, 400, GLFW_DONT_CARE, GLFW_DONT_CARE);

        // Sets the window as the current OpenGL context
        glfwMakeContextCurrent(window);

        // Sets the number of frames between buffer swaps
        glfwSwapInterval(1);

        // Starts the graphics loop
        graphicsLoop();
    }

    void graphicsLoop() {

        // Resetting the camera perspective
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Executed until it's time for the window to close
        while (!glfwWindowShouldClose(window)) {

            // All drawing is done here
            draw();



            // Swapping the frame buffers
            glfwSwapBuffers(window);

            // Responding to input
            glfwPollEvents();
        }

        // Closing the viewport when the program is closed
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }

    void draw() {

        // Sets the background to black
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        // Clear information and sets up the 3d model world
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_POLYGON_SMOOTH);
        glEnable(GL_BLEND);
        glLoadIdentity();

        // Testing
        vec3 testCirclePosition = vec3(0, 0, -1);
        drawCircle(testCirclePosition, 3);
    }

    static void drawCircle(vec3 position, float radius) {

        glDepthRange(0.5, 1000);

        // Number of segments affects how 'smooth' the circle will be.
        int numSegments = 36;

        glBegin(GL_TRIANGLE_FAN);

        for (int i = 0; i < numSegments; ++i) {

            float angle = (2.0f * 3.1415926f * float(i)) / float(numSegments);

            float x = radius * cosf(angle);
            float y = radius * sinf(angle);
            glVertex3f(x + position.x, y + position.y, position.z);
        }

        glEnd();
    }

    static void handleError(int error, const char *description) {
        fprintf(stderr, "Error: %s\n", description);
    }

    static void handleResize(GLFWwindow *window, int width, int height) {

        // Getting the size of the window
        float ratio = width / (float) height;

        // Setting the window to the right size
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // Setting the camera perspective to match the window aspect ratio
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity(); // Resets the camera

        // Resetting the camera perspective
    }

private:

    // The window
    GLFWwindow *window;

    // The simulation
    simulationState *theSim;

};


#endif //N_BODY_HEADLESS_VIEWPORT_H
