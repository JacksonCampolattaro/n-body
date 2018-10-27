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

    viewport(int width = 1000, int height = 1000, const char *title = "n_Body Simulator 0.2.0") {

        // Setting the function for handling errors
        glfwSetErrorCallback(error_callback);

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

        // Setting the window's minimum size
        glfwSetWindowSizeLimits(window, 400, 400, GLFW_DONT_CARE, GLFW_DONT_CARE);

        // Sets the window as the current OpenGL context
        glfwMakeContextCurrent(window);

        // Sets the number of frames between buffer swaps
        glfwSwapInterval(1);


    }

    void graphicsLoop(simulationState *theSim) {

        // Executed until it's time for the window to close
        while (!glfwWindowShouldClose(window)) {

            // Getting the size of the window
            float ratio;
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            ratio = width / (float) height;

            // Setting the window to the right size
            glViewport(0, 0, width, height);
            glClear(GL_COLOR_BUFFER_BIT);

            // Setting the camera perspective to match the window aspect ratio
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity(); // Resets the camera
            //gluPerspective( 45.0f, ratio, 1.0f, 100.0f );

            //All drawing is done here
            draw(theSim);

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

    void draw(simulationState *theSim) {

        // Sets the background to black
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        // Removes all vertices from the last draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
        glLoadIdentity(); //Reset the drawing perspective

        vec3 testCirclePosition = vec3(0, 0, -50);
        drawCircle(testCirclePosition, 20);
    }

    static void error_callback(int error, const char *description) {
        fprintf(stderr, "Error: %s\n", description);
    }

    static void drawCircle(vec3 position, float radius) {

        // Number of segments defines how 'smooth' the circle needs to be. these are far away, so it doesn't need to be too high.
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

private:

    GLFWwindow *window;

};


#endif //N_BODY_HEADLESS_VIEWPORT_H
