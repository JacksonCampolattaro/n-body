//
// Created by jackcamp on 6/18/19.
//

#ifndef N_BODY_RENDERER_H
#define N_BODY_RENDERER_H


#include "../../tools/Position.h"
#include "../../tools/Color.h"

class Renderer {

public:

    /**
     * Any Renderer will have to know how to draw itself
     */
    virtual void draw() = 0;

private:

    Position position;

    Color color;
};


#endif //N_BODY_RENDERER_H
