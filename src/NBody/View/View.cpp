//
// Created by jackcamp on 7/17/21.
//

#include <spdlog/spdlog.h>
#include <glibmm/main.h>

#include <thread>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/RenderbufferFormat.h>

#include "View.h"

NBody::View::View(NBody::Camera &camera, const NBody::Simulation &simulation) :
        Gtk::GLArea(), _simulation(simulation), _camera(camera) {

    set_size_request(720, 480);

    set_required_version(3, 2);
    set_has_depth_buffer(true);

    signal_realize().connect(sigc::mem_fun(*this, &View::onRealize));
    signal_unrealize().connect(sigc::mem_fun(*this, &View::onUnrealize));
    signal_render().connect(sigc::mem_fun(*this, &View::onRender), true);
    signal_resize().connect(sigc::mem_fun(*this, &View::onResize));

    slot_renderNeeded = sigc::mem_fun(*this, &View::requestRender);
    _camera.signal_changed().connect(slot_renderNeeded);
    _simulation.signal_changed.connect(slot_renderNeeded);
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
    spdlog::trace("Rendering");

    // Eliminate outside effects on GL state
    GL::Context::current().resetState(GL::Context::State::ExitExternal);

    // Determine the relevant framebuffer by getting its ID
    GLint framebufferID;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferID);

    // Attach Magnum's framebuffer manager to the framebuffer provided by Gtkmm
    auto gtkmmDefaultFramebuffer = GL::Framebuffer::wrap(
            framebufferID,
            {{},
             {get_allocated_width(), get_allocated_height()}}
    );

    _camera.draw(_simulation, gtkmmDefaultFramebuffer);

//    GL::Texture2D color;
//    GL::Renderbuffer depthStencil;
//    color.setStorage(1, GL::TextureFormat::RGBA8, {100, 100});
//    depthStencil.setStorage(GL::RenderbufferFormat::Depth24Stencil8, {100, 100});
//    GL::Framebuffer framebuffer{{{}, {100, 100}}};
//    framebuffer.attachTexture(GL::Framebuffer::ColorAttachment{0}, color, 0);
//    framebuffer.attachRenderbuffer(GL::Framebuffer::BufferAttachment::DepthStencil, depthStencil);
//    _camera.draw(_simulation, framebuffer);
//    DebugTools::screenshot(framebuffer, "/Users/jackcamp/Documents/n_body/scenarios/test2.tga");

    // Restore external GL state
    GL::Context::current().resetState(GL::Context::State::EnterExternal);

    signal_doneRendering.emit();
    return true;
}

void NBody::View::onResize(int width, int height) {
    spdlog::trace("Resizing window");
    queue_render();
}

void NBody::View::requestRender() {
    spdlog::trace("Render requested");

    // Wait a moment before triggering the next frame, otherwise this doesn't seem to work
    Glib::signal_timeout().connect_once([&]() {
        queue_render();
    }, 8);
}
