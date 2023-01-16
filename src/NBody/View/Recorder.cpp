//
// Created by Jackson Campolattaro on 8/26/22.
//

#include "Recorder.h"

Glib::RefPtr<Gdk::Pixbuf> NBody::toPixbuf(const Image2D &image) {
    auto data = reinterpret_cast<const guint8 *>(image.data().data());
    return Gdk::Pixbuf::create_from_data(
            data, Gdk::Colorspace::RGB, true, 8,
            image.size().x(), image.size().y(),
            (int) image.pixels().stride()[0]
    )->flip(false);
}

NBody::Recorder::Recorder(NBody::Camera &camera, RendererList renderers, sigc::signal<void()> &trigger) :
        _camera(camera), _renderers(renderers), _trigger(trigger),
        _context(Gdk::Display::get_default()->create_gl_context()) {
}

Glib::RefPtr<Gdk::Pixbuf>  NBody::Recorder::takeImage(const Vector2i &resolution) {
    return NBody::toPixbuf(snapshot(resolution));
}

Image2D NBody::Recorder::snapshot(const Vector2i &resolution) {
    _context->make_current();

    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    GL::Context::current().resetState(GL::Context::State::ExitExternal);

    // Create a framebuffer, and add color and depth stencil buffers
    GL::Framebuffer framebuffer{{{}, resolution}};
    GL::Texture2D color;
    color.setStorage(1, GL::TextureFormat::RGBA8, resolution);
    framebuffer.attachTexture(GL::Framebuffer::ColorAttachment{0}, color, 0);
    GL::Renderbuffer depthStencil;
    depthStencil.setStorage(GL::RenderbufferFormat::Depth24Stencil8, resolution);
    framebuffer.attachRenderbuffer(GL::Framebuffer::BufferAttachment::DepthStencil, depthStencil);

    // Draw to the newly created framebuffer
    for (auto renderer: _renderers)
        renderer.get().draw(_camera.transformationMatrix(), _camera.projectionMatrix(framebuffer));

    // Return the color buffer, as an image
    return color.image(0, {GL::PixelFormat::RGBA, GL::PixelType::UnsignedByte});
}

void NBody::Recorder::startVideo(const std::string &filePath, const Vector2i &resolution, int frameRate) {

    _outputStream = std::make_unique<VideoOutputStream>(filePath, resolution, frameRate);

    // Take the first frame of the video
    _outputStream->writeFrame(snapshot(resolution));
    signal_recordedFrame.emit();

    // Collect another frame each time the simulation steps
    _connection = _trigger.connect([&]() {
        _outputStream->writeFrame(snapshot({_outputStream->frame->width, _outputStream->frame->height}));
        signal_recordedFrame.emit();
    });
}

void NBody::Recorder::stopVideo() {
    _connection.disconnect();
    _connection = {};
    _outputStream.reset();
}
