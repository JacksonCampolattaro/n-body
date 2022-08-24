//
// Created by Jackson Campolattaro on 8/23/22.
//

#ifndef N_BODY_INTENTRY_H
#define N_BODY_INTENTRY_H


#include <gtkmm/spinbutton.h>
#include <gtkmm/builder.h>

namespace UI {

    template<typename IntType>
    class IntEntry : public Gtk::SpinButton {

    public: // Signals

        sigc::signal<void(IntType)> signal_changed;

    public: // Slots

        sigc::slot<void(IntType)> slot_changed;

    public:

        IntEntry(Gtk::SpinButton::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) :
                Gtk::SpinButton(cobject) {

            set_vexpand(false);
            set_valign(Gtk::Align::CENTER);

            set_digits(0);
            set_width_chars(10);
            set_range(std::numeric_limits<IntType>::lowest(), std::numeric_limits<IntType>::max());
            set_increments(1.0, 10.0);

            signal_value_changed().connect([&]() {
                signal_changed.emit((IntType) get_value_as_int());
            });

            slot_changed = [&](IntType v) {
                set_value(v);
            };

        }

        void setValue(IntType v) {
            set_value(v);
        }

        IntType getValue() const {
            return get_value_as_int();
        }
    };

    template<typename IntType>
    class SimpleIntEntry : public IntEntry<IntType> {
    public:

        SimpleIntEntry(Gtk::SpinButton::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) :
                IntEntry<IntType>(cobject, builder) {

            // Hide +/- buttons, to use the SpinButton as a simple numeric entry
            this->get_last_child()->hide();
            this->get_last_child()->get_prev_sibling()->hide();
        }
    };

    typedef SimpleIntEntry<std::size_t> SimpleSizeEntry;

};


#endif //N_BODY_INTENTRY_H
