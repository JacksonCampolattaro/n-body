//
// Created by jackcamp on 2/21/22.
//

#include "VectorEditable.h"

UI::FloatEntry::FloatEntry() : Gtk::SpinButton() {

    set_digits(9);
    set_width_chars(10);
    set_hexpand();

    set_range(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    set_increments(0.1, 1.0);
}
