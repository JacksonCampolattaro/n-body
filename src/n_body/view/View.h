//
// Created by jackcamp on 4/10/19.
//

#ifndef N_BODY_VIEW_H
#define N_BODY_VIEW_H

#include <glm/glm.hpp> // Include all of glm, because I'm handling many different types of vectors
#include <vector> // Allows me to keep a list of things to draw
#include "Drawable.h"

#include <raylib.h>

class View {
public:

    View(int width, int height, const char *title);

    void createWindow();

    void loop();

    void draw(std::vector<Drawable> drawables);

private:

    int width;
    int height;
    const char *title;

    Camera camera = { 0 };

};


#endif //N_BODY_VIEW_H
