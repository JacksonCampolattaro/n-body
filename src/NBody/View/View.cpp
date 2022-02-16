//
// Created by jackcamp on 7/17/21.
//

#include "View.h"

NBody::View::View(NBody::ArcBallCamera &camera, const NBody::Simulation &simulation) :
        Gtk::GLArea(), _simulation(simulation), _camera(camera) {

    set_size_request(400, 400);

    //set_auto_render();

    set_required_version(4, 5);
    set_has_depth_buffer(true);

    signal_realize().connect(sigc::mem_fun(*this, &View::onRealize));
    signal_unrealize().connect(sigc::mem_fun(*this, &View::onUnrealize));
    signal_render().connect(sigc::mem_fun(*this, &View::onRender), true);
    signal_resize().connect(sigc::mem_fun(*this, &View::onResize));

    slot_renderNeeded = sigc::mem_fun(*this, &View::queue_render);
    _camera.signal_changed.connect(slot_renderNeeded);
}

void NBody::View::onRealize() {

    // Initialize the GL context
    make_current();
    _context.create();

    // Configure the graphics features
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    // Set the background color
    GL::Renderer::setClearColor({0.12, 0.12, 0.1, 1.0});
}

bool NBody::View::onRender(const Glib::RefPtr<Gdk::GLContext> &) {

    // Eliminate outside effects on GL state
    GL::Context::current().resetState(GL::Context::State::ExitExternal);

    // Determine the relevant framebuffer by getting its ID
    GLint framebufferID;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferID);

    // Attach Magnum's framebuffer manager to the framebuffer provided by Gtkmm
    auto gtkmmDefaultFramebuffer = GL::Framebuffer::wrap(
            framebufferID,
            {{},
             {get_width(), get_height()}}
    );

    // Reset color and depth buffers
    gtkmmDefaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    _camera.updateTransformation();
    _camera.draw(_simulation);

    // Restore external GL state
    GL::Context::current().resetState(GL::Context::State::EnterExternal);
    return true;
}

void NBody::View::onResize(int width, int height) {
    _camera.reshape(Vector2i{get_allocated_width(), get_allocated_height()});
    queue_render();
}
