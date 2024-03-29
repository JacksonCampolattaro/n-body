//
// Created by Jackson Campolattaro on 6/26/22.
//

#ifndef N_BODY_PARTICLECOLORENTRY_H
#define N_BODY_PARTICLECOLORENTRY_H

#include "Application/UI/Widgets/Bindable.h"

#include "NBody/Graphics/Color.h"

#include <gtkmm/colorbutton.h>
#include <gtkmm/builder.h>

namespace UI {

    using NBody::Graphics::Color;

    class ParticleColorEntry : public Bindable<Color>, public Gtk::ColorButton {
    public:

        ParticleColorEntry(Gtk::ColorButton::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : Gtk::ColorButton(cobject) {

            set_rgba(Gdk::RGBA{"lightblue"});

            signal_color_set().connect([&]() {
                auto rgba = get_rgba();
                changed(Color{rgba.get_red(), rgba.get_green(), rgba.get_blue()});
            });
        }

        void update(const Color &value) override {
            set_sensitive(true);
            set_rgba({value.r(), value.g(), value.b()});
        }
        void update() override {
            set_sensitive(false);
        };
    };
}

#endif //N_BODY_PARTICLECOLORENTRY_H
