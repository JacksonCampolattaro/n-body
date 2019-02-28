//
// Created by jackcamp on 2/27/19.
//

#include "viewport.h"

#include <GL/glu.h> // Might already be included with GLFW
#include <cstdio> // Used for outputting errors GLFW encounters

viewport::viewport() = default;

viewport *viewport::setDimensions(glm::ivec2 dimensions) {

    this->dimensions = dimensions;

    return this;
}

viewport *viewport::setTitle(const char *title) {

    this->title = title;

    return this;
}

/*viewport *viewport::attachSimulation(simulation *simulation) {

    this->simulation = simulation;

    return this;
}*/

viewport *viewport::registerDrawable(drawable *drawable) {

    this->drawables.push_back(drawable);

    return this;
}


viewport *viewport::attachSimulation(simulation *theSim) {

    this->theSim = theSim;

    return this;
}

void viewport::start() {

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

    graphicsLoop();
}

void viewport::setColor(glm::vec3 color) {

    // Sets the color used in the next OpenGL action
    glColor3f(color.r, color.g, color.b);
}

void viewport::drawCircle(glm::vec3 position, float radius) {

    // Number of segments affects how 'smooth' the circle will be.
    int numSegments = 64;

    glBegin(GL_TRIANGLE_FAN);

    for (int i = 0; i < numSegments; ++i) {

        float angle = (2.0f * 3.1415926f * float(i)) / float(numSegments);

        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex3f(x + position.x, y + position.y, position.z);
    }

    glEnd();

}

void viewport::handleError(int error, const char *description) {

    fprintf(stderr, "Error: %s\n", description);
}

void viewport::handleResize(GLFWwindow *window, int width, int height) {

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

void viewport::graphicsLoop() {

    // TODO May have to be reworked to incorporate flags

    // Executed until it's time for the window to close
    while (!glfwWindowShouldClose(window)) {

        // All drawing is done here
        draw();

        // TODO I'm not sure which class should control the other
        theSim->increment();

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

void viewport::draw() {

    // TODO This boilerplate could probably be rewritten

    // Sets the background to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Clear information and sets up the 3d model world
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glLoadIdentity();

    // Calls the draw function of every registered drawable
    // TODO There might be advantages to multithreading this, it depends on if the GPU is the limiting factor
    for (int d = 0; d < drawables.size(); ++d) {
        drawables[d]->draw();
    }
}

