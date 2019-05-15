//
// Created by jackcamp on 5/8/19.
//

#ifndef N_BODY_VIEWPORT_H
#define N_BODY_VIEWPORT_H


#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>
#include "Observer.h"

class ViewPort : public Observer {

public:

    // Constructors

    ViewPort();


    // Setters (with support for chaining)

    /**
     * Configures the shape of the window
     * @param dimensions Vector representing the <X, Y> dimensions of the window
     * @return This viewport, for use in chaining named parameters.
     */
    ViewPort *setDimensions(glm::ivec2 dimensions) override;

    /**
     * Sets the title of the window
     * @param title The new title
     * @return This viewport, for use in chaining named parameters.
     */
    ViewPort *setTitle(const char *title) override;


    // Methods used externally

    void draw(std::vector<Drawable *> drawables) override;


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
    const char *title = "GLFW Viewport";


    // Parameters affecting image quality

    /*Multisampling prevents jagged, pixellated edges on diagonal lines and curves*/
    bool multisamplingEnabled = true;
    unsigned int samplingRatio = 8;



};


#endif //N_BODY_VIEWPORT_H
