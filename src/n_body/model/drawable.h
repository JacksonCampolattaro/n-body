//
// Created by jackcamp on 10/28/18.
//

#ifndef N_BODY_HEADLESS_DRAWABLE_H
#define N_BODY_HEADLESS_DRAWABLE_H


class drawable {

public:

    /**
     * Drawables are registered to the graphics function
     * When each drawable must have a method for drawing itself.
     */
    virtual void draw() = 0;

};


#endif //N_BODY_HEADLESS_DRAWABLE_H
