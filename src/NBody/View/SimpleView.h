//
// Created by jackcamp on 7/17/21.
//

#ifndef N_BODY_SIMPLEVIEW_H
#define N_BODY_SIMPLEVIEW_H

#include <optional>

#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>
#include <Magnum/Math/Color.h>
#include <Magnum/DebugTools/Screenshot.h>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderer.h>

#include <NBody/Simulation/Simulation.h>

#include "View.h"

using namespace Magnum;
using namespace Math::Literals;

namespace NBody {

    class SimpleView : public View {
    protected:

        const Simulation &_simulation;

        Camera &_camera;

        void setup();

    public:

        SimpleView(Camera &camera, const Simulation &simulation);

        SimpleView(Gtk::GLArea::BaseObjectType *cobject,
                   const Glib::RefPtr<Gtk::Builder> &builder,
                   Camera &camera, const Simulation &simulation);

        void onRender(GL::Framebuffer &defaultFramebuffer) override;

    };
}


#endif //N_BODY_SIMPLEVIEW_H
