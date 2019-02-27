//
// Created by jackcamp on 2/26/19.
//

#ifndef N_BODY_BETTER_VIEWPORT_H
#define N_BODY_BETTER_VIEWPORT_H


#include <glm/glm.hpp>
#include <string>

#include "drawable.h"
#include "../model/better_simulation.h"

/**
 * Improved viewport system
 * I'm using the fluent interface paradigm to make it simpler to construct.
 */
class better_viewport {

public:

    // Constructor

    /**
     * Creates the viewport with default parameters
     */
    explicit better_viewport() = default;


    // Setters (with support for chaining)

    /**
     * Configures the shape of the window
     * @param dimensions Vector representing the <X, Y> dimensions of the window
     * @return This viewport, for use in chaining named parameters.
     */
    better_viewport *setDimensions(glm::ivec2 dimensions);

    /**
     * Sets the title of the window
     * @param title The new title
     * @return This viewport, for use in chaining named parameters.
     */
    better_viewport *setTitle(const char *title);


    // Setup tools

    better_viewport *attachSimulation(better_simulation *simulation);

    /**
     * Adds a new item to the render to be drawn each cycle
     * @param newDrawable The new drawable to be added
     */
    better_viewport *registerDrawable(drawable *drawable);


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
    GLFWwindow *window;

    /*Vector representing the <X, Y> dimensions of the window*/
    glm::ivec2 dimensions;

    /*The label of the window*/
    const char *title = "n_Body Simulator 0.3.0";


    // References the model and associated drawables

    /*A reference is kept to the simulation being rendered*/
    better_simulation *simulation;

    /*The list of drawables that will be put on the screen each time it's updated*/
    std::vector<drawable *> drawables;


    // Parameters which define the interface - model relationship

    // TODO This should be mutable with flags


    // Parameters affecting image quality

    /*Multisampling prevents jagged, pixellated edges on diagonal lines and curves*/
    bool multisamplingEnabled = true;
    unsigned int samplingRatio = 4;


    // Helper methods

    /*Infinite loop handling graphics*/
    void graphicsLoop();

    /*Draws all the drawables to the screen*/
    void draw();






};


#endif //N_BODY_BETTER_VIEWPORT_H
