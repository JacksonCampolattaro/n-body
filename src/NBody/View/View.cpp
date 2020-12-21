//
// Created by jackcamp on 12/21/20.
//

#include "View.h"

View::View::View() : Gtk::GLArea() {

    set_auto_render();

    set_required_version(4, 5);

    signal_realize().connect(sigc::mem_fun(this, &View::realize));
    signal_render().connect(sigc::mem_fun(this, &View::render));
    signal_resize().connect(sigc::mem_fun(this, &View::resize));
    signal_unrealize().connect(sigc::mem_fun(this, &View::unrealize));
}

void View::View::draw() {

}

void View::View::realize() {
    Gtk::GLArea::realize();
}

bool View::View::render(const Glib::RefPtr<Gdk::GLContext> &context) {
    return false;
}

void View::View::resize(int width, int height) {

}

void View::View::unrealize() {
    Gtk::GLArea::unrealize();
}
