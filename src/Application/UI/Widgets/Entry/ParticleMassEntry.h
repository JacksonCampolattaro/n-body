//
// Created by Jackson Campolattaro on 6/26/22.
//

#ifndef N_BODY_PARTICLEMASSENTRY_H
#define N_BODY_PARTICLEMASSENTRY_H

#include "Application/UI/Widgets/Bindable.h"
#include "FloatEntry.h"

#include "NBody/Physics/Mass.h"

namespace UI {

    using NBody::Physics::Mass;

    class ParticleMassEntry : public Bindable<Mass>, public FloatEntry {
    public:

        ParticleMassEntry(Gtk::SpinButton::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : FloatEntry(cobject, builder) {

            signal_changed.connect([&](float v) {
                changed(Mass(v));
            });
        }

        void update(const Mass &value) override {
            set_sensitive(true);
            setValue(value.mass());
        }

        void update() override {
            set_sensitive(false);
        };
    };
}

#endif //N_BODY_PARTICLEMASSENTRY_H
