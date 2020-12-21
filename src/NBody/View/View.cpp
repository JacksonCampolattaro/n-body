//
// Created by jackcamp on 12/21/20.
//

#include "View.h"

View::View::View() : Gtk::GLArea() {

    set_auto_render();

    set_required_version(4, 5);

    signal_realize().connect(sigc::mem_fun(this, &View::setupGL));
    signal_render().connect(sigc::mem_fun(this, &View::render));
    signal_resize().connect(sigc::mem_fun(this, &View::resize));
}

void View::View::draw(GL::Framebuffer &framebuffer) {

    framebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

}

void View::View::setupGL() {
    Widget::realize();

    make_current();
    _context.create();

    set_has_depth_buffer();
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
}

bool View::View::render(const Glib::RefPtr<Gdk::GLContext> &context) {

    GL::Context::current().resetState(GL::Context::State::ExitExternal);

    GLint framebufferID;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferID);
    auto framebuffer =
            GL::Framebuffer::wrap(framebufferID, {{}, {get_width(), get_height()}});

    draw(framebuffer);

    GL::Context::current().resetState(GL::Context::State::EnterExternal);

    return false;
}

void View::View::resize(int width, int height) {

}
