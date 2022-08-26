//
// Created by Jackson Campolattaro on 8/26/22.
//

#include "Recorder.h"


Glib::RefPtr<Gdk::Pixbuf> NBody::toPixbuf(GL::Texture2D &texture) {
    auto image = texture.image(0, {GL::PixelFormat::RGB, GL::PixelType::UnsignedByte});
    auto data = reinterpret_cast<guint8 *>(image.data().data());
    return Gdk::Pixbuf::create_from_data(
            data, Gdk::Colorspace::RGB, false, 8,
            image.size().x(), image.size().y(),
            (int) image.pixels().stride()[0]
    )->flip(false);
}

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

    NBody::toPixbuf(color)->save("/Users/jackcamp/Documents/n_body/scenarios/test.png", "png");
}
