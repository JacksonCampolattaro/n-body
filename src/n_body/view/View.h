//
// Created by jackcamp on 4/10/19.
//

#ifndef N_BODY_VIEW_H
#define N_BODY_VIEW_H


#include <GLFW/glfw3.h> // GLFW makes OpenGL simpler to use
#include <glm/glm.hpp> // Include all of glm, because I'm handling many different types of vectors
#include <vector> // Allows me to keep a list of things to draw
#include "Drawable.h"

class View {
public:

    // Constructor

    /**
     * Creates the viewport with default parameters
     */
    explicit View();


    // Setters (with support for chaining)

    /**
     * Configures the shape of the window
     * @param dimensions Vector representing the <X, Y> dimensions of the window
     * @return This viewport, for use in chaining named parameters.
     */
    View *setDimensions(glm::ivec2 dimensions);

    /**
     * Sets the title of the window
     * @param title The new title
     * @return This viewport, for use in chaining named parameters.
     */
    View *setTitle(const char *title);


    // Setup tools

    /**
     * Adds a new item to the render to be drawn each cycle
     * @param newDrawable The new drawable to be added
     */
    View *registerDrawable(Drawable *newDrawable);

    /**
     * Adds a list of items to the render to be drawn each cycle
     * @param newDrawables The new drawable list to be added
     */
    View *registerDrawables(std::vector<Drawable *> newDrawables);


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

    /**
     * Draws a circle in 3d space
     * @param position The location of the circle
     * @param radius The size of the circle
     */
    static void drawSphere(glm::vec3 position, float radius);



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


    // Helper methods

    /*Draws all the drawables to the screen, then updates the buffer*/
    void draw();


private:

    // Parameters defining the window

    /*A reference to the window itself*/
    GLFWwindow *window = nullptr;

    /*Vector representing the <X, Y> dimensions of the window*/
    glm::ivec2 dimensions = glm::ivec2(1920, 1080);

    /*The label of the window*/
    const char *title = "GLFW Viewport";


    // Parameters affecting image quality

    /*Multisampling prevents jagged, pixellated edges on diagonal lines and curves*/
    bool multisamplingEnabled = true;
    unsigned int samplingRatio = 8;


    // References to items to be drawn

    /*The list of drawables that will be put on the screen each time it's updated*/
    std::vector<Drawable *> drawables;

};


#endif //N_BODY_VIEW_H
