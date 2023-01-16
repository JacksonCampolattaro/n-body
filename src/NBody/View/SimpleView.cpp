//
// Created by jackcamp on 7/17/21.
//

#include <spdlog/spdlog.h>
#include <glibmm/main.h>

#include <thread>
#include <utility>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/RenderbufferFormat.h>

#include "SimpleView.h"

NBody::SimpleView::SimpleView(Gtk::GLArea::BaseObjectType *cobject,
                              const Glib::RefPtr<Gtk::Builder> &builder,
                              NBody::Camera &camera,
                              RendererList renderers) :
        View(cobject, builder), _camera(camera), _renderers(std::move(renderers)) {

    set_size_request(720, 480);

    _camera.signal_changed().connect(slot_renderNeeded());
    for (auto renderer: _renderers)
        renderer.get().signal_changed().connect(slot_renderNeeded());
}

void NBody::SimpleView::onRender(GL::Framebuffer &defaultFramebuffer) {

    defaultFramebuffer.bind();

    // Set the background color
    GL::Renderer::setClearColor(_camera.getBackgroundColor());

    // Reset color and depth buffers
    defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    for (auto renderer: _renderers)
        renderer.get().draw(_camera.transformationMatrix(), _camera.projectionMatrix(defaultFramebuffer));
}
