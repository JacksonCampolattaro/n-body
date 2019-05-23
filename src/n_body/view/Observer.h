//
// Created by jackcamp on 5/8/19.
//

#ifndef N_BODY_OBSERVER_H
#define N_BODY_OBSERVER_H


#include <vector>
#include <glm/detail/type_vec.hpp>
#include "Drawable.h"

class Observer {
public:

    /**
     * Renders a list of drawables to the output context using their own draw functions.
     * @param drawables A list of drawable items passed by value to allow for asynchronous rendering.
     */
    virtual void draw(std::vector<Drawable *> drawables) = 0;


    // Setters (with support for chaining)

    /**
     * Configures the shape of the window
     * @param dimensions Vector representing the <X, Y> dimensions of the window
     * @return This viewport, for use in chaining named parameters.
     */
    virtual Observer *setDimensions(glm::ivec2 dimensions) = 0;

    /**
     * Sets the title of the window
     * @param title The new title
     * @return This viewport, for use in chaining named parameters.
     */
    virtual Observer *setTitle(const char *title) = 0;

};


#endif //N_BODY_OBSERVER_H
