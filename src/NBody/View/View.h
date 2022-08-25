//
// Created by jackcamp on 7/17/21.
//

#ifndef N_BODY_VIEW_H
#define N_BODY_VIEW_H

#include <optional>

#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>
#include <Magnum/Math/Color.h>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderer.h>

#include <NBody/Simulation/Simulation.h>

#include "Camera.h"

using namespace Magnum;
using namespace Math::Literals;

namespace NBody {

    class View : public Gtk::GLArea {
    protected:

        const Simulation &_simulation;

        Platform::GLContext _context{NoCreate, 0, nullptr};

        Camera &_camera;

    public:

        View(Camera &camera, const Simulation &simulation);

        void onRealize();

        void onUnrealize() {}

        bool onRender(const Glib::RefPtr<Gdk::GLContext> &);

        void onResize(int width, int height);

        void requestRender();

    public: // Signals

        sigc::signal<void()> signal_doneRendering;

    public: // Slots

        sigc::slot<void()> slot_renderNeeded;

    };
}


#endif //N_BODY_VIEW_H
