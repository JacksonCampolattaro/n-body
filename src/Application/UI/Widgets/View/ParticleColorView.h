//
// Created by Jackson Campolattaro on 6/26/22.
//

#ifndef N_BODY_PARTICLECOLORVIEW_H
#define N_BODY_PARTICLECOLORVIEW_H

#include "Application/UI/Widgets/Bindable.h"

#include "NBody/Graphics/Color.h"

#include <gtkmm/colorbutton.h>
#include <gtkmm/builder.h>

namespace UI {

    using NBody::Graphics::Color;

    class ParticleColorView : public Bindable<Color>, public Gtk::Label {
    public:

        ParticleColorView() {
            set_use_markup();
        }

        void update(const Color &value) override {
            show();

            auto hexString = fmt::format(
                    "#{:02x}{:02x}{:02x}",
                    (int) (value.r() * 255),
                    (int) (value.g() * 255),
                    (int) (value.b() * 255)
            );

            this->set_label(fmt::format(
                    R"(<span font_desc="monospace" underline="double" underline_color="{}">{}</span>)",
                    hexString, hexString
            ));
        }

        void update() override { hide(); };
    };
}

#endif //N_BODY_PARTICLECOLORVIEW_H
