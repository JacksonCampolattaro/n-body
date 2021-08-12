//
// Created by jackcamp on 7/17/21.
//

#ifndef N_BODY_VIEW_H
#define N_BODY_VIEW_H

#include "Camera.h"

#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>
#include <Magnum/Math/Color.h>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderer.h>

using namespace Magnum;
using namespace Math::Literals;

namespace NBody {

    class View : public Gtk::GLArea {
    private:

        Platform::GLContext _context{NoCreate, 0, nullptr};
        const Simulation::Simulation &_simulation;
        Camera _camera;

    public:

        View(const Simulation::Simulation &simulation) : Gtk::GLArea(), _simulation(simulation), _camera() {

            set_auto_render();

            set_required_version(4, 5);

            signal_realize().connect(sigc::mem_fun(this, &View::onRealize));
            signal_unrealize().connect(sigc::mem_fun(this, &View::onUnrealize));
            signal_render().connect(sigc::mem_fun(this, &View::onRender));
            signal_resize().connect(sigc::mem_fun(this, &View::onResize));
        };

        void onRealize() {

            // Initialize the GL context
            make_current();
            _context.create();

            // Configure the graphics features
            GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
            GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

            // Set the background color
            GL::Renderer::setClearColor({0.12, 0.12, 0.1, 1.0});
        }

        void onUnrealize() {

        }

        bool onRender(const Glib::RefPtr<Gdk::GLContext> &) {

            // Eliminate outside effects on GL state
            GL::Context::current().resetState(GL::Context::State::ExitExternal);

            // Determine the relevant framebuffer by getting its ID
            GLint framebufferID;
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferID);

            // Attach Magnum's framebuffer manager to the framebuffer provided by Gtkmm
            auto gtkmmDefaultFramebuffer = GL::Framebuffer::wrap(
                    framebufferID,
                    {{},{get_width(), get_height()}}
            );

            // Reset color and depth buffers
            gtkmmDefaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

            // TODO: This is where actual rendering code goes!
            _camera.draw(_simulation);

            // Restore external GL state
            GL::Context::current().resetState(GL::Context::State::EnterExternal);
            return true;
        }

        void onResize(int width, int height) {
            _camera.setAspectRatio((float) width / (float) height);
        }

    };
}


#endif //N_BODY_VIEW_H
