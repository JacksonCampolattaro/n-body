//
// Created by jackcamp on 7/17/21.
//

#ifndef N_BODY_VIEW_H
#define N_BODY_VIEW_H

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

    public:

        View() : Gtk::GLArea() {

            set_auto_render();

            set_required_version(4, 5);

            signal_realize().connect(sigc::mem_fun(this, &View::onRealize));
            signal_unrealize().connect(sigc::mem_fun(this, &View::onUnrealize));
            signal_render().connect(sigc::mem_fun(this, &View::onRender));
        };

        void onRealize() {

            // Initialize the GL context
            make_current();
            _context.create();

            // Configure the graphics features
            GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
            GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

            // Set the background color
            GL::Renderer::setClearColor(Math::Color4<float>::red());
        }

        void onUnrealize() {

        }

        bool onRender(const Glib::RefPtr<Gdk::GLContext> &context) {

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

            // Restore external GL state
            GL::Context::current().resetState(GL::Context::State::ExitExternal);
            return false;
        }

    };
}


#endif //N_BODY_VIEW_H
