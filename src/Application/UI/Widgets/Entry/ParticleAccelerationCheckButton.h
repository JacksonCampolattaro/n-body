//
// Created by Jackson Campolattaro on 1/30/23.
//

#ifndef N_BODY_PARTICLEACCELERATIONCHECKBUTTON_H
#define N_BODY_PARTICLEACCELERATIONCHECKBUTTON_H

#include "Application/UI/Widgets/Bindable.h"

#include "NBody/Physics/Acceleration.h"

#include <gtkmm/checkbutton.h>
#include <gtkmm/builder.h>

namespace UI {

    using NBody::Physics::Acceleration;

    class ParticleAccelerationCheckButton : public Bindable<Acceleration>, public Gtk::CheckButton {
    public:

        ParticleAccelerationCheckButton(Gtk::CheckButton::BaseObjectType *cobject,
                                        const Glib::RefPtr<Gtk::Builder> &builder) : Gtk::CheckButton(cobject) {
            signal_toggled().connect([&]() {
                if (get_active()) changed<Acceleration>({0.0f, 0.0f, 0.0f});
                else changed<Acceleration>();
            }, true);
        }

        void update(const Acceleration &value) override { set_active(true); }

        void update() override { set_active(false); };
    };
}

#endif //N_BODY_PARTICLEACCELERATIONCHECKBUTTON_H
