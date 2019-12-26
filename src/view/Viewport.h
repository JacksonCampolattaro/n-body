//
// Created by jackcamp on 12/25/19.
//

#ifndef N_BODY_VIEWPORT_H
#define N_BODY_VIEWPORT_H


#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/GLContext.h>

#include "../model/tools/BodyList.h"

class Viewport {

public:

    Viewport(BodyList *bodies);

    void refresh();
};


#endif //N_BODY_VIEWPORT_H
