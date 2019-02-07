//
// Created by jackcamp on 10/25/18.
//

#include "viewport.h"

viewport::viewport(simulation *theSim, int width, int height, const char *title) {

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

    // Enabling antialiasing
    glEnable(GL_MULTISAMPLE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    // Setting the function for handling window resizing
    glfwSetWindowSizeCallback(window, handleResize);

    // Setting the window's minimum size
    glfwSetWindowSizeLimits(window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);

    // Sets the window as the current OpenGL context
    glfwMakeContextCurrent(window);

    // Sets the number of frames between buffer swaps
    glfwSwapInterval(1);

    // Sets the perspective before beginning the loop
    handleResize(window, width, height);
}

void viewport::registerDrawable(drawable *newDrawable) {

    // Adds the new item to the array
    drawables.push_back(newDrawable);
}

void viewport::graphicsLoop() {

    // Executed until it's time for the window to close
    while (!glfwWindowShouldClose(window)) {

        // All drawing is done here
        draw();

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

    // Sets the background to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Clear information and sets up the 3d model world
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glLoadIdentity();

    for (auto theDrawable : drawables) {
        theDrawable->draw();
    }
}

void viewport::setColor(vec3 color) {
    glColor3f(color.r, color.g, color.b);
}

void viewport::drawCircle(vec3 position, float radius) {

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

void viewport::handleError(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void viewport::handleResize(GLFWwindow *window, int width, int height) {

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


