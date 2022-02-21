//
// Created by jackcamp on 2/18/22.
//

#include <spdlog/spdlog.h>
#include "PositionEntry.h"

UI::FloatEntry::FloatEntry() : Gtk::SpinButton() {

    set_digits(9);
    set_width_chars(10);
    set_hexpand();

    set_range(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    set_increments(0.1, 1.0);
}

UI::PositionEntry::PositionEntry() : Gtk::Button() {

    get_style_context()->add_class("popup-entry");

    set_child(_preview);

    _popoverGrid.attach(_xLabel, 0, 0);
    _popoverGrid.attach(_xEntry, 1, 0);
    _popoverGrid.attach(_yLabel, 0, 1);
    _popoverGrid.attach(_yEntry, 1, 1);
    _popoverGrid.attach(_zLabel, 0, 2);
    _popoverGrid.attach(_zEntry, 1, 2);

    _popoverGrid.set_column_spacing(8);
    _popoverGrid.set_row_spacing(4);
    _popover.set_child(_popoverGrid);

    _popover.set_parent(*this);

    signal_clicked().connect(sigc::mem_fun(_popover, &Gtk::Popover::popup));

    slot_update = sigc::mem_fun(*this, &UI::PositionEntry::on_update);

    _xEntry.signal_value_changed().connect(sigc::mem_fun(*this, &UI::PositionEntry::on_valueChanged));
    _yEntry.signal_value_changed().connect(sigc::mem_fun(*this, &UI::PositionEntry::on_valueChanged));
    _zEntry.signal_value_changed().connect(sigc::mem_fun(*this, &UI::PositionEntry::on_valueChanged));

    on_valueChanged();
}

void UI::PositionEntry::on_valueChanged() {

    _preview.setValue(_xEntry.get_value(), _yEntry.get_value(), _zEntry.get_value());
    signal_changed.emit(_xEntry.get_value(), _yEntry.get_value(), _zEntry.get_value());
}

void UI::PositionEntry::on_update(double x, double y, double z) {
    _xEntry.set_value(x);
    _yEntry.set_value(y);
    _zEntry.set_value(z);
}
