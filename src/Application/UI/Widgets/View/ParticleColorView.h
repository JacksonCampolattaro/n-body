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

    class ParticleColorView : public Bindable<Color>, public Gtk::ColorButton {
    public:

        ParticleColorView() {
            set_sensitive(false);
        }

        void update(Color &value) override {
            set_rgba({value.r(), value.g(), value.b()});
        }
    };
}

#endif //N_BODY_PARTICLECOLORVIEW_H
