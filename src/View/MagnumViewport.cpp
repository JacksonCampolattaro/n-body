//
// Created by jackcamp on 2/4/20.
//

#include "MagnumViewport.h"

View::MagnumViewport::MagnumViewport() :
    Gtk::GLArea(),
    _context(Magnum::NoCreate) {

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

}

bool View::MagnumViewport::onRender(const Glib::RefPtr<Gdk::GLContext> &context) {
    return false;
}

void View::MagnumViewport::onResize(int width, int height) {

}

void View::MagnumViewport::onUnrealize() {

}
