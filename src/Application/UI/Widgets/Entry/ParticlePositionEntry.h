//
// Created by Jackson Campolattaro on 6/26/22.
//

#ifndef N_BODY_PARTICLEPOSITIONENTRY_H
#define N_BODY_PARTICLEPOSITIONENTRY_H

#include "Application/UI/Widgets/Bindable.h"

#include "VectorEntry.h"

#include "NBody/Physics/Position.h"

namespace UI {

    using NBody::Physics::Position;

    class ParticlePositionEntry : public Bindable<Position>, public CompactPositionEntry {
    public:

        ParticlePositionEntry(Button::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : CompactVectorEntry(cobject, builder) {

            signal_changed.connect([&](float x, float y, float z) {
                changed(Position(x, y, z));
            });
        }

        void update(const Position &value) override {
            show();
            setValue(value);
        }

        void update() override { hide(); };
    };
}

#endif //N_BODY_PARTICLEPOSITIONENTRY_H
