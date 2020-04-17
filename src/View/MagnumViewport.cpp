//
// Created by jackcamp on 2/4/20.
//

#include "MagnumViewport.h"

View::MagnumViewport::MagnumViewport() :
    Gtk::GLArea(),
    _context(NoCreate) {

    spdlog::trace("MagnumViewport constructor invoked");

    // Automatically re-render everything each time it needs to be redrawn
    set_auto_render();

    // Set desired OpenGL version
    set_required_version(4, 5);

    // Connecting signals to their respective handlers
    signal_realize().connect(sigc::mem_fun(this, &MagnumViewport::onRealize));
    signal_render().connect(sigc::mem_fun(this, &MagnumViewport::onRender));
    signal_resize().connect(sigc::mem_fun(this, &MagnumViewport::onResize));
    signal_unrealize().connect(sigc::mem_fun(this, &MagnumViewport::onUnrealize));
}

View::MagnumViewport::~MagnumViewport() {}

void View::MagnumViewport::onRealize() {

    spdlog::trace("MagnumViewport onRealize invoked");

    // Make the OpenGL context current, and then configure it
    make_current();
    _context.create();
}

bool View::MagnumViewport::onRender(const Glib::RefPtr<Gdk::GLContext> &context) {

    spdlog::trace("MagnumViewport onRender invoked");

    // Reset state, so Gtkmm's defaults won't interfere with Magnum
    GL::Context::current().resetState(GL::Context::State::ExitExternal);

    // Get the ID of the current framebuffer
    GLint framebufferID;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferID);

    // Attach Magnum's framebuffer manager to the framebuffer provided by Gtkmm
    auto framebuffer =
            GL::Framebuffer::wrap(framebufferID, {{}, {get_width(), get_height()}});

    // Clear
    framebuffer.clear(GL::FramebufferClear::Color);

    // Rendering
    // TODO

    // Undo Magnum's changes to the graphics state
    Magnum::GL::Context::current().resetState(GL::Context::State::EnterExternal);;
    return false;
}

void View::MagnumViewport::onResize(int width, int height) {

    spdlog::trace("MagnumViewport onResize invoked");

    // TODO
}

void View::MagnumViewport::onUnrealize() {

    spdlog::trace("MagnumViewport onUnrealize invoked");

    // TODO
}
