//
// Created by Jackson Campolattaro on 6/26/22.
//

#ifndef N_BODY_PARTICLEVELOCITYENTRY_H
#define N_BODY_PARTICLEVELOCITYENTRY_H

#include "Application/UI/Widgets/Bindable.h"
#include "../Entry/VectorEntry.h"

#include <NBody/Physics/Position.h>

namespace UI {

    using NBody::Physics::Velocity;

    class ParticleVelocityEntry : public Bindable<Velocity>, public CompactVelocityEntry {
    public:

        ParticleVelocityEntry(Button::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : CompactVectorEntry(cobject, builder) {

            signal_changed.connect([&](float x, float y, float z) {
                changed(Velocity(x, y, z));
            });
        }

        void update(const Velocity &value) override {
            show();
            setValue(value);
        }

        void update() override { hide(); };
    };
}

#endif //N_BODY_PARTICLEVELOCITYENTRY_H
