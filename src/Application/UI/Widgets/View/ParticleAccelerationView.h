//
// Created by Jackson Campolattaro on 1/30/23.
//

#ifndef N_BODY_PARTICLEACCELERATIONVIEW_H
#define N_BODY_PARTICLEACCELERATIONVIEW_H

#include "Application/UI/Widgets/View/VectorView.h"
#include "Application/UI/Widgets/Bindable.h"
#include "NBody/Simulation/Simulation.h"

namespace UI {

    class ParticleAccelerationView : public AccelerationView, public Bindable<NBody::Physics::Acceleration> {
    public:

        ParticleAccelerationView(Gtk::Box::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : AccelerationView(cobject, builder) {}

        void update(const NBody::Physics::Acceleration &value) override {
            set_sensitive(true);
            setValue(value);
        };

        void update() override {
            set_sensitive(false);
            setValue({0.0f, 0.0f, 0.0f});
        };
    };

}

#endif //N_BODY_PARTICLEACCELERATIONVIEW_H
