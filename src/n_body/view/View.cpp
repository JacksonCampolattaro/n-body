//
// Created by jackcamp on 4/10/19.
//

#include <cstdio>
#include <GL/glu.h>
#include "View.h"

View::View() {

    // TODO Boilerplate code that creates the window could be improved

    // Initializing GLFW
    if (!glfwInit()) {
        // Exits if GLFW Fails to load
        exit(EXIT_FAILURE);
    }

    // Setting the function for handling errors
    glfwSetErrorCallback(handleError);

    if (multisamplingEnabled) {
        // Enabling antialiasing
        glEnable(GL_MULTISAMPLE);
        glfwWindowHint(GLFW_SAMPLES, samplingRatio);
    }

    // Creating the window
    window = glfwCreateWindow(dimensions.x, dimensions.y, title, nullptr, nullptr);
    if (!window) {
        // Kills the program if the window fails to load
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Setting the function for handling window resizing
    glfwSetWindowSizeCallback(window, handleResize);

    // Setting the window's minimum size
    glfwSetWindowSizeLimits(window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);

    // Sets the window as the current OpenGL context
    glfwMakeContextCurrent(window);

    // Sets the number of frames between buffer swaps
    glfwSwapInterval(1);

    // Sets the perspective before beginning the loop
    handleResize(window, dimensions.x, dimensions.y);
}

View *View::setDimensions(glm::ivec2 dimensions) {

    this->dimensions = dimensions;
    glfwSetWindowSize(window, dimensions.x, dimensions.y);

    return this;
}

const glm::ivec2 &View::getDimensions() const {
    return dimensions;
}

View *View::setTitle(const char *title) {

    glfwSetWindowTitle(window, title);

    return this;
}

View *View::registerDrawable(Drawable *newDrawable) {

    this->drawables.push_back(newDrawable);

    return this;
}

View *View::registerDrawables(std::vector<Drawable *> newDrawables) {

    this->drawables.insert(this->drawables.end(), newDrawables.begin(), newDrawables.end());

    return this;
}

void View::handleError(int error, const char *description) {

    fprintf(stderr, "Error: %s\n", description);
}

void View::handleResize(GLFWwindow *window, int width, int height) {

    // TODO Must be reworked before enabling camera movement

    // Getting the aspect ratio of the window
    float ratio = width / (float) height;

    // Setting the window to the right size
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    // Setting the camera perspective to match the window aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // Resets the camera

    // Resetting the camera perspective
    gluPerspective(60, ratio, 0.01, 10000);
}

void View::draw() {

    // Closing the Viewport when the program is closed
    if (glfwWindowShouldClose(window)) {

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    // Sets the background to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Clear information and sets up the 3d Model world
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glLoadIdentity();

    // Calls the draw function of every registered Drawable
    // TODO There might be advantages to multithreading this, it depends on if the GPU is the limiting factor
    for (int d = 0; d < drawables.size(); ++d) {
        this->drawables[d]->draw();
    }

    // Swapping the frame buffers
    glfwSwapBuffers(window);

    // Responding to input
    glfwPollEvents();
}

void View::draw(std::vector<Drawable *> drawables) {

    // Closing the Viewport when the program is closed
    if (glfwWindowShouldClose(window)) {

        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }

    // Otherwise, perform standard draw operations

    // Sets the background to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Clear information and sets up the 3d Model world
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glLoadIdentity();

    // Calls the draw function of every registered Drawable
    // TODO There might be advantages to multithreading this, it depends on if the GPU is the limiting factor
    for (int d = 0; d < drawables.size(); ++d) {
        drawables[d]->draw();
    }

    // Swapping the frame buffers
    glfwSwapBuffers(window);

    // Responding to input
    glfwPollEvents();
}
