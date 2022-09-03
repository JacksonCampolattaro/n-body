//
// Created by jackcamp on 8/15/21.
//

#ifndef N_BODY_INTERACTIVEVIEW_H
#define N_BODY_INTERACTIVEVIEW_H

#include "SimpleView.h"

#include <gtkmm/glarea.h>
#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/eventcontrollermotion.h>
#include <gtkmm/gestureclick.h>

#include <Magnum/Platform/GLContext.h>
#include <Magnum/Math/Color.h>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderer.h>

#include <optional>

using namespace Magnum;
using namespace Math::Literals;

namespace NBody {

    class InteractiveView : public SimpleView {
    private:

        Magnum::Vector2 toNDC(float x, float y);

    public:

        InteractiveView(Gtk::GLArea::BaseObjectType *cobject,
                        const Glib::RefPtr<Gtk::Builder> &builder,
                        ControllableCamera &camera, Simulation &simulation);
    };

}


#endif //N_BODY_INTERACTIVEVIEW_H
