//
// Created by Jackson Campolattaro on 1/30/23.
//

#ifndef N_BODY_PARTICLEMASSCHECKBUTTON_H
#define N_BODY_PARTICLEMASSCHECKBUTTON_H

#include "Application/UI/Widgets/Bindable.h"

#include "NBody/Physics/Mass.h"

#include <gtkmm/checkbutton.h>
#include <gtkmm/builder.h>

namespace UI {

    using NBody::Physics::Mass;

    class ParticleMassCheckButton : public Bindable<Mass>, public Gtk::CheckButton {
    public:

        ParticleMassCheckButton(Gtk::CheckButton::BaseObjectType *cobject,
                                const Glib::RefPtr<Gtk::Builder> &builder) : Gtk::CheckButton(cobject) {
            signal_toggled().connect([&]() {
                if (get_active()) changed<Mass>({1.0f});
                else changed<Mass>();
            }, true);
        }

        void update(const Mass &value) override { set_active(true); }

        void update() override { set_active(false); };
    };
}

#endif //N_BODY_PARTICLEMASSCHECKBUTTON_H
