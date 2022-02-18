//
// Created by jackcamp on 2/18/22.
//

#include <spdlog/spdlog.h>
#include "PositionEntry.h"

UI::PositionEntry::PositionEntry() : Gtk::Button() {

    get_style_context()->add_class("popup-entry");

    _previewLabels[1].set_width_chars(7);
    _previewLabels[3].set_width_chars(7);
    _previewLabels[5].set_width_chars(7);
    _previewLabels[1].set_halign(Gtk::Align::END);
    _previewLabels[3].set_halign(Gtk::Align::END);
    _previewLabels[5].set_halign(Gtk::Align::END);
//    _previewLabels[1].set_justify(Gtk::Justification::RIGHT);
//    _previewLabels[3].set_justify(Gtk::Justification::RIGHT);
//    _previewLabels[5].set_justify(Gtk::Justification::RIGHT);
//    _previewLabels[1].set_hexpand();
//    _previewLabels[3].set_hexpand();
//    _previewLabels[5].set_hexpand();
    for (auto &label : _previewLabels)
        _previewBox.append(label);
    set_child(_previewBox);

    _xEntry.set_digits(9);
    _yEntry.set_digits(9);
    _zEntry.set_digits(9);
    _xEntry.set_width_chars(10);
    _yEntry.set_width_chars(10);
    _zEntry.set_width_chars(10);
    _xEntry.set_range(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    _yEntry.set_range(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    _zEntry.set_range(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    _xEntry.set_hexpand();
    _xEntry.set_increments(0.1, 1.0);
    _yEntry.set_increments(0.1, 1.0);
    _zEntry.set_increments(0.1, 1.0);

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

    std::stringstream stream;
    stream << std::setprecision(2);// << std::scientific;

    stream.str(std::string());
    stream << _xEntry.get_value();
    _previewLabels[1].set_label(stream.str());
    stream.clear();

    stream.str(std::string());
    stream << _yEntry.get_value();
    _previewLabels[3].set_label(stream.str());
    stream.clear();

    stream.str(std::string());
    stream << _zEntry.get_value();
    _previewLabels[5].set_label(stream.str());
    stream.clear();

    signal_changed.emit(_xEntry.get_value(), _yEntry.get_value(), _zEntry.get_value());
}

void UI::PositionEntry::on_update(double x, double y, double z) {
    _xEntry.set_value(x);
    _yEntry.set_value(y);
    _zEntry.set_value(z);
}
