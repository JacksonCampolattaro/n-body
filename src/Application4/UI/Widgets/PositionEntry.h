//
// Created by jackcamp on 2/18/22.
//

#ifndef N_BODY_POSITIONENTRY_H
#define N_BODY_POSITIONENTRY_H

#include <array>

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/popover.h>
#include <gtkmm/entry.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/grid.h>
#include <gtkmm/adjustment.h>

#include <NBody/Physics/Position.h>

#include "VectorView.h"

namespace UI {

    class FloatEntry : public Gtk::SpinButton {
    public:

        FloatEntry();

    };

    class PositionEntry : public Gtk::Button {
    private:

        PositionView _preview;

        Gtk::Popover _popover;
        Gtk::Grid _popoverGrid;
        Gtk::Label _xLabel{"X"}, _yLabel{"Y"}, _zLabel{"Z"};
        FloatEntry _xEntry, _yEntry, _zEntry;

    public:
        // Slots

        sigc::slot<void(double, double, double)> slot_update;

    public:
        // Signals

        sigc::signal<void(double, double, double)> signal_changed;

    public:

        PositionEntry();

        void on_valueChanged();

        void on_update(double x, double y, double z);
    };

}

#endif //N_BODY_POSITIONENTRY_H
