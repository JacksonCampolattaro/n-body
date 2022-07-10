//
// Created by Jackson Campolattaro on 6/27/22.
//

#ifndef N_BODY_PARTICLEICONVIEW_H
#define N_BODY_PARTICLEICONVIEW_H

#include <spdlog/spdlog.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/aspectframe.h>

#include "NBody/Graphics/Color.h"
#include "NBody/Graphics/Sphere.h"

#include "../Bindable.h"

namespace UI {

    class ParticleIconView
            : public Gtk::AspectFrame,
              public Bindable<NBody::Graphics::Sphere, NBody::Graphics::Color> {
    private:

        Gtk::DrawingArea _drawingArea;

        Gdk::RGBA _color;
        float _radius;

    public:

        ParticleIconView() : Gtk::AspectFrame() {

            _drawingArea.set_draw_func(sigc::mem_fun(*this, &ParticleIconView::on_draw));
            set_child(_drawingArea);

            set_margin(0);
            set_expand();

            _drawingArea.set_margin(0);
            _drawingArea.set_expand();
        }

        void update(NBody::Graphics::Sphere &sphere, NBody::Graphics::Color &color) override {
            show();

            _color = {color.r(), color.g(), color.b()};
            float diameter = sphere.radius() / (1.0f + sphere.radius());
            _radius = diameter / 2.0f;

            _drawingArea.set_content_height(get_height());
            _drawingArea.set_content_width(get_height());

            // This is silly, but it seems to be the only way to guarantee that the icon gets re-drawn
            queue_draw();
            hide();
            show();

            spdlog::trace("Radius {} mapped to radius {} in particle icon view", sphere.radius(), _radius);
        };

        void update() override { hide(); };

    protected:

        void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {

            const int xc = width / 2;
            const int yc = height / 2;

//            cr->rectangle(0, 0, width, height);
//            cr->set_source_rgba(0.0, 0.0, 0.0, 1.0);
//            cr->fill();

            cr->arc(xc, yc, width * _radius / 2, 0.0, 2.0 * M_PI);
            cr->set_source_rgb(_color.get_red(), _color.get_green(), _color.get_blue());
            cr->fill_preserve();
            cr->stroke();
        }
    };


}

#endif //N_BODY_PARTICLEICONVIEW_H
