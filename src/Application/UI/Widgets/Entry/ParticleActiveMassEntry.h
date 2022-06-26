//
// Created by Jackson Campolattaro on 6/26/22.
//

#ifndef N_BODY_PARTICLEACTIVEMASSENTRY_H
#define N_BODY_PARTICLEACTIVEMASSENTRY_H

#include "Application/UI/Widgets/Bindable.h"
#include "FloatEntry.h"

#include "NBody/Physics/ActiveMass.h"

namespace UI {

    using NBody::Physics::ActiveMass;

    class ParticleActiveMassEntry : public Bindable<ActiveMass>, public FloatEntry {
    public:

        ParticleActiveMassEntry(Gtk::SpinButton::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : FloatEntry(cobject, builder) {

            signal_changed.connect([&](float v) {
                changed(ActiveMass(v));
            });
        }

        void update(ActiveMass &value) override { setValue(value.mass()); }
    };
}

#endif //N_BODY_PARTICLEACTIVEMASSENTRY_H
