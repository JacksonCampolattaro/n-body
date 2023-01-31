//
// Created by Jackson Campolattaro on 1/30/23.
//

#ifndef N_BODY_PARTICLEACCELERATIONENTRY_H
#define N_BODY_PARTICLEACCELERATIONENTRY_H

#include "Application/UI/Widgets/Bindable.h"
#include "../Entry/VectorEntry.h"

#include <NBody/Physics/Position.h>

namespace UI {

    using NBody::Physics::Acceleration;

    class ParticleAccelerationEntry : public Bindable<Acceleration>, public CompactAccelerationEntry {
    public:

        ParticleAccelerationEntry(Button::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : CompactVectorEntry(cobject, builder) {

            signal_changed.connect([&](float x, float y, float z) {
                changed(Acceleration{x, y, z});
            });
        }

        void update(const Acceleration &value) override {
            set_sensitive(true);
            setValue(value);
        }

        void update() override {
            set_sensitive(false);
            setValue({0.0f, 0.0f, 0.0f});
        };
    };
}


#endif //N_BODY_PARTICLEACCELERATIONENTRY_H
