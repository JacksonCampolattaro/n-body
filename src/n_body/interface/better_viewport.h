//
// Created by jackcamp on 2/26/19.
//

#ifndef N_BODY_BETTER_VIEWPORT_H
#define N_BODY_BETTER_VIEWPORT_H


#include <glm/glm.hpp>
#include <string>

#include "../model/drawable.h"

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
    better_viewport *setDimensions(glm::vec2 dimensions);

    /**
     * Sets the title of the window
     * @param title The new title
     * @return This viewport, for use in chaining named parameters.
     */
    better_viewport *setTitle(std::string title);


    // Setup tools

    /**
     * Adds a new item to the render to be drawn each cycle
     * @param newDrawable The new drawable to be added
     */
    void registerDrawable(drawable *newDrawable);


    // Graphics tools

    /**
     * Allows for more easily setting the color in OpenGL
     * @param color The color as a vector (rgb)
     */
    static void setColor(glm::vec3 color);

    /**
     * Draws a circle in 3d space
     * @param position The location of the circle
     * @param radius The size of the circle
     */
    static void drawCircle(glm::vec3 position, float radius);

private:


};


#endif //N_BODY_BETTER_VIEWPORT_H
