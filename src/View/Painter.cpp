//
// Created by jackcamp on 9/12/20.
//

#include "Painter.h"

View::Painter::Painter() {

}

void View::Painter::set_glarea(Gtk::GLArea &glArea) {

}

void View::Painter::draw(const std::deque<Model::Drawable::Drawable> &drawables) {

}

void View::Painter::on_realize() {

}

bool View::Painter::on_render(const Glib::RefPtr<Gdk::GLContext> &context) {
    return false;
}

void View::Painter::on_resize(int width, int height) {

}

void View::Painter::on_unrealize() {

}
