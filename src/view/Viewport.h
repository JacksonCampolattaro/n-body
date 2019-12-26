//
// Created by jackcamp on 12/25/19.
//

#ifndef N_BODY_VIEWPORT_H
#define N_BODY_VIEWPORT_H


#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/GLContext.h>
#include <Magnum/Platform/Sdl2Application.h>

#include "../model/tools/BodyList.h"

class Viewport : public Magnum::Platform::Application {
public:
    explicit Viewport(const Arguments &arguments);

private:
    void drawEvent() override;
};


#endif //N_BODY_VIEWPORT_H
