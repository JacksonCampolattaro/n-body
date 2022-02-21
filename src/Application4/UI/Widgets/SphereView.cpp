//
// Created by jackcamp on 2/21/22.
//

#include "SphereView.h"

#include <spdlog/spdlog.h>

UI::SphereView::SphereView() : Gtk::AspectFrame() {

    _drawingArea.add_css_class("sphere-view");
    _drawingArea.set_overflow(Gtk::Overflow::HIDDEN);

    //_drawingArea.set_size_request(48, 48);
    //_drawingArea.set_hexpand();

    _drawingArea.set_draw_func(sigc::mem_fun(*this, &SphereView::on_draw));
    set_child(_drawingArea);
}

void UI::SphereView::setValue(const NBody::Graphics::Color &color, const NBody::Graphics::Sphere &sphere) {
    _color = color;
    _radius = sphere.radius() / (1 + sphere.radius());
    spdlog::debug("Radius {} mapped to radius {} in preview", sphere.radius(), _radius);
}

void UI::SphereView::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height) {

    const int xc = width / 2;
    const int yc = height / 2;

    cr->rectangle(0, 0, width, height);
    cr->set_source_rgba(0.0, 0.0, 0.0, 1.0);
    cr->fill();

    cr->arc(xc, yc, width * _radius / 2, 0.0, 2.0 * M_PI);
    cr->set_source_rgba(_color.r(), _color.g(), _color.b(), 1.0);
    cr->fill_preserve();
    cr->stroke();
}
