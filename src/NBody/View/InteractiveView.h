//
// Created by jackcamp on 8/15/21.
//

#ifndef N_BODY_INTERACTIVEVIEW_H
#define N_BODY_INTERACTIVEVIEW_H

#include "View.h"

#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>
#include <Magnum/Math/Color.h>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderer.h>

#include <optional>

#include "GtkmmArcBallCamera.h"

using namespace Magnum;
using namespace Math::Literals;

namespace NBody {

    class InteractiveView : public View {
    private:

        Magnum::Vector2 toNDC(float x, float y);

    public:

        InteractiveView(GtkmmArcBallCamera &camera, Simulation &simulation);
    };

}


#endif //N_BODY_INTERACTIVEVIEW_H
