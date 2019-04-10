//
// Created by jackcamp on 2/27/19.
//

#ifndef N_BODY_VIEWPORT_H
#define N_BODY_VIEWPORT_H


#include "Drawable.h" // Objects this class draws are all extensions of Drawable
#include "../model/Simulation.h" // Fix before adding

#include <GLFW/glfw3.h> // GLFW makes OpenGL simpler to use
#include <glm/glm.hpp> // Include all of glm, because I'm handling many different types of vectors
#include <vector> // Allows me to keep a list of things to draw

class Viewport {

public:

    // Constructor

    /**
     * Creates the viewport with default parameters
     */
    explicit Viewport();


    // Setters (with support for chaining)

    /**
     * Configures the shape of the window
     * @param dimensions Vector representing the <X, Y> dimensions of the window
     * @return This viewport, for use in chaining named parameters.
     */
    Viewport *setDimensions(glm::ivec2 dimensions);

    /**
     * Sets the title of the window
     * @param title The new title
     * @return This viewport, for use in chaining named parameters.
     */
    Viewport *setTitle(const char *title);


    // Setup tools

    /**
     * Adds a new item to the render to be drawn each cycle
     * @param newDrawable The new drawable to be added
     */
    Viewport *registerDrawable(Drawable *drawable);

    Viewport *attachSimulation(Simulation *theSim);


    // Controller tools

    /**
     * Creates the window and starts the graphics loop
     */
    void start();


    // Graphics tools

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


    // GLFW boilerplate

    /**
     * GLFW's method for reporting errors
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

    // Parameters defining the window

    /*A reference to the window itself*/
    GLFWwindow *window = nullptr;

    /*Vector representing the <X, Y> dimensions of the window*/
    glm::ivec2 dimensions = glm::ivec2(1920, 1080);

    /*The label of the window*/
    const char *title = "n_Body Simulator 0.3.0";


    // References the Model and associated drawables

    /*A reference is kept to the Simulation being rendered*/
    //Simulation *theSim;

    /*The list of drawables that will be put on the screen each time it's updated*/
    std::vector<Drawable *> drawables;


    // Parameters which define the interface - Model relationship

    // TODO This should be mutable with flags
    Simulation *theSim;


    // Parameters affecting image quality

    /*Multisampling prevents jagged, pixellated edges on diagonal lines and curves*/
    bool multisamplingEnabled = true;
    unsigned int samplingRatio = 8;


    // Helper methods

    /*Infinite loop handling graphics*/
    void graphicsLoop();

    /*Draws all the drawables to the screen*/
    void draw();

};


#endif //N_BODY_VIEWPORT_H
