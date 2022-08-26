//
// Created by Jackson Campolattaro on 8/26/22.
//

#include "Recorder.h"

#include <gtkmm.h>


NBody::Recorder::Recorder(NBody::Camera &camera, const NBody::Simulation &simulation) :
        _camera(camera), _simulation(simulation), _context(Gdk::Display::get_default()->create_gl_context()) {

}

void NBody::Recorder::takeImage(const Vector2i &resolution) {
    spdlog::debug("Taking a screenshot");

    _context->make_current();

    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    GL::Context::current().resetState(GL::Context::State::ExitExternal);

    GL::Texture2D color;
    GL::Renderbuffer depthStencil;
    color.setStorage(1, GL::TextureFormat::RGBA8, resolution);
    depthStencil.setStorage(GL::RenderbufferFormat::Depth24Stencil8, resolution);

    GL::Framebuffer framebuffer{{{}, resolution}};
    framebuffer.attachTexture(GL::Framebuffer::ColorAttachment{0}, color, 0);
    framebuffer.attachRenderbuffer(GL::Framebuffer::BufferAttachment::DepthStencil, depthStencil);

    _camera.draw(_simulation, framebuffer);

    DebugTools::screenshot(framebuffer, "/Users/jackcamp/Documents/n_body/scenarios/test.tga");

    GL::Context::current().resetState(GL::Context::State::EnterExternal);
}
