//
// Created by Jackson Campolattaro on 6/26/22.
//

#ifndef N_BODY_PARTICLERADIUSENTRY_H
#define N_BODY_PARTICLERADIUSENTRY_H

#include "Bindable.h"
#include "../Entry/FloatEntry.h"

#include <NBody/Graphics/Sphere.h>

namespace UI {

    using NBody::Graphics::Sphere;

    class ParticleRadiusEntry : public Bindable<Sphere>, public FloatEntry {
    public:

        ParticleRadiusEntry(Gtk::SpinButton::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : FloatEntry(cobject, builder) {

            signal_changed.connect([&](float v) {
                changed(Sphere(v));
            });
        }

        void update(Sphere &value) override { setValue(value.radius()); }
    };
}

#endif //N_BODY_PARTICLERADIUSENTRY_H
