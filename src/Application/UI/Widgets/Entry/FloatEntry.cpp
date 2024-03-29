//
// Created by jackcamp on 2/22/22.
//

#include "FloatEntry.h"

void UI::FloatEntry::setup() {
    set_digits(4);
    set_width_chars(10);

    set_vexpand(false);
    set_valign(Gtk::Align::CENTER);

    set_range(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    set_increments(0.1, 1.0);

    signal_value_changed().connect([&]() {
        signal_changed.emit((float) get_value());
    });

    slot_changed = [&](float v) {
        set_value(v);
    };
}

UI::FloatEntry::FloatEntry() : Gtk::SpinButton() {
    setup();
}

UI::FloatEntry::FloatEntry(Gtk::SpinButton::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) :
        Gtk::SpinButton(cobject) {
    setup();
}

void UI::FloatEntry::setValue(float v) {
    set_value(v);
}

void UI::PositiveFloatEntry::setup() {
    set_digits(3);
    set_width_chars(10);

    set_vexpand(false);
    set_valign(Gtk::Align::CENTER);

    set_range(0, std::numeric_limits<float>::max());
    set_increments(0.01, 0.1);

    signal_value_changed().connect([&]() {
        signal_changed.emit((float) get_value());
    });

    slot_changed = [&](float v) {
        set_value(v);
    };
}

UI::PositiveFloatEntry::PositiveFloatEntry() : Gtk::SpinButton() {
    setup();
}

UI::PositiveFloatEntry::PositiveFloatEntry(Gtk::SpinButton::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) :
        Gtk::SpinButton(cobject) {
    setup();
}

void UI::PositiveFloatEntry::setValue(float v) {
    set_value(v);
}
