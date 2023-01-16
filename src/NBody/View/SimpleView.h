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
#include <NBody/Simulation/Solvers/MultiSolver.h>

#include "View.h"
#include "NBody/View/Renderer.h"
#include "NBody/View/Renderers/SolverRenderer.h"

#include <functional>
#include <vector>

using namespace Magnum;
using namespace Math::Literals;

namespace NBody {

    class SimpleView : public View {
    protected:

        Camera &_camera;

        RendererList _renderers;

    public:

        SimpleView(Gtk::GLArea::BaseObjectType *cobject,
                   const Glib::RefPtr<Gtk::Builder> &builder,
                   Camera &camera, RendererList renderers);

        void onRender(GL::Framebuffer &defaultFramebuffer) override;

    };
}


#endif //N_BODY_SIMPLEVIEW_H
