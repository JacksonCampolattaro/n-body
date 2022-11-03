//
// Created by jackcamp on 2/22/22.
//

#ifndef N_BODY_FLOATENTRY_H
#define N_BODY_FLOATENTRY_H

#include <gtkmm/spinbutton.h>
#include <gtkmm/builder.h>



//namespace std {
//    template<>
//    class numeric_limits<PositiveFloat> {
//    public:
//        static int max() { return std::numeric_limits<float>::max(); };
//    };
//}

namespace UI {

    class FloatEntry : public Gtk::SpinButton {
    protected:
        void setup();

    public: // Signals

        sigc::signal<void(float)> signal_changed;

    public: // Slots

        sigc::slot<void(float)> slot_changed;

    public:

        FloatEntry();

        FloatEntry(Gtk::SpinButton::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder);

        void setValue(float v);
    };

    class PositiveFloatEntry : public Gtk::SpinButton {
    protected:
        void setup();

    public: // Signals

        sigc::signal<void(float)> signal_changed;

    public: // Slots

        sigc::slot<void(float)> slot_changed;

    public:

        PositiveFloatEntry();

        PositiveFloatEntry(Gtk::SpinButton::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder);

        void setValue(float v);
    };

}

#endif //N_BODY_FLOATENTRY_H
