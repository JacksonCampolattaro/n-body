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

#include "SimpleView.h"

NBody::SimpleView::SimpleView(NBody::Camera &camera, const NBody::Simulation &simulation) :
        View(), _simulation(simulation), _camera(camera) {

    set_size_request(720, 480);

    _camera.signal_changed().connect(slot_renderNeeded());
    _simulation.signal_changed.connect(slot_renderNeeded());
}

void NBody::SimpleView::onRender(GL::Framebuffer &defaultFramebuffer) {
    _camera.draw(_simulation, defaultFramebuffer);
}
