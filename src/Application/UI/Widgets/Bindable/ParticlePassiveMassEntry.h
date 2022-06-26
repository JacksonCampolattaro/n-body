//
// Created by Jackson Campolattaro on 6/26/22.
//

#ifndef N_BODY_PARTICLEPASSIVEMASSENTRY_H
#define N_BODY_PARTICLEPASSIVEMASSENTRY_H

#include "Bindable.h"
#include "../Entry/FloatEntry.h"

#include <NBody/Physics/PassiveMass.h>

namespace UI {

    using NBody::Physics::PassiveMass;

    class ParticlePassiveMassEntry : public Bindable<PassiveMass>, public FloatEntry {
    public:

        ParticlePassiveMassEntry(Gtk::SpinButton::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : FloatEntry(cobject, builder) {

            signal_changed.connect([&](float v) {
                changed(PassiveMass(v));
            });
        }

        void update(PassiveMass &value) override { setValue(value.mass()); }
    };
}

#endif //N_BODY_PARTICLEPASSIVEMASSENTRY_H
