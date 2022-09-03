//
// Created by Jackson Campolattaro on 8/26/22.
//

#ifndef N_BODY_VIEW_H
#define N_BODY_VIEW_H

#include <optional>

#include <gtkmm/glarea.h>
#include <gtkmm/builder.h>

#include <Magnum/Platform/GLContext.h>
#include <Magnum/Math/Color.h>
#include <Magnum/DebugTools/Screenshot.h>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderer.h>

#include <NBody/Simulation/Simulation.h>

#include "Camera.h"

namespace NBody {

    using namespace Magnum;
    using namespace Math::Literals;

    class View : public Gtk::GLArea {
    protected:

        sigc::signal<void()> _signal_doneRendering;

        sigc::slot<void()> _slot_renderNeeded;

        Platform::GLContext _context{NoCreate, 0, nullptr};

        void setup() {

            set_required_version(3, 2);
            set_has_depth_buffer(true);

            _slot_renderNeeded = sigc::mem_fun(*this, &View::requestRender);

            signal_realize().connect([&]() {
                make_current();
                _context.create();
            });
        }

    public:

        View() {
            setup();
        }

        View(Gtk::GLArea::BaseObjectType *cobject,
             const Glib::RefPtr<Gtk::Builder> &builder) :
                Gtk::GLArea(cobject) {
            setup();
        }

        virtual void onRender(GL::Framebuffer &defaultFramebuffer) = 0;

        void requestRender() {
            spdlog::trace("Render requested");

            // Wait a moment before triggering the next frame, otherwise this doesn't seem to work
            Glib::signal_timeout().connect_once([&]() {
                queue_render();
            }, 8);
        };

    public: // Signals

        sigc::signal<void()> &signal_doneRendering() { return _signal_doneRendering; };

    public: // Slots

        sigc::slot<void()> &slot_renderNeeded() { return _slot_renderNeeded; };

    protected:

        bool on_render(const Glib::RefPtr<Gdk::GLContext> &_) override {
            spdlog::trace("Rendering");

            // Eliminate outside effects on GL state
            GL::Context::current().resetState(GL::Context::State::ExitExternal);

            // Determine the relevant framebuffer by getting its ID
            GLint framebufferID;
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferID);

            // Attach Magnum's framebuffer manager to the framebuffer provided by Gtkmm
            auto defaultFramebuffer = GL::Framebuffer::wrap(
                    framebufferID,
                    {{},
                     {get_allocated_width() * get_scale_factor(), get_allocated_height() * get_scale_factor()}}
            );

            // Invoke the subclass's render function
            onRender(defaultFramebuffer);

            // Restore external GL state
            GL::Context::current().resetState(GL::Context::State::EnterExternal);

            _signal_doneRendering.emit();
            return true;
        }

        void on_resize(int width, int height) override {
            queue_render();
        }

    };
}


#endif //N_BODY_VIEW_H
