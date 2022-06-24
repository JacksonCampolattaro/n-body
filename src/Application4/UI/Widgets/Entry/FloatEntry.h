//
// Created by jackcamp on 2/22/22.
//

#ifndef N_BODY_FLOATENTRY_H
#define N_BODY_FLOATENTRY_H

#include <gtkmm/spinbutton.h>
#include <gtkmm/builder.h>

namespace UI {

    class FloatEntry : public Gtk::SpinButton {
    private:

        void setup();

    public: // Signals

        sigc::signal<void(float)> signal_changed;

    public: // Slots

        sigc::slot<void(float)> slot_changed;

    public:

        FloatEntry();

        FloatEntry(Gtk::SpinButton::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder);
    };

}

#endif //N_BODY_FLOATENTRY_H
