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

namespace UI {

    class VectorEntry : public Gtk::Button {

    public:

        VectorEntry(char open = '(', char close = ')');
    };

    class PositionEntry : public Gtk::Button {
    private:

        Gtk::Box _previewBox;
        std::array<Gtk::Label, 7> _previewLabels{
                Gtk::Label{"("},
                Gtk::Label{"X"},
                Gtk::Label{", "},
                Gtk::Label{"Y"},
                Gtk::Label{", "},
                Gtk::Label{"Z"},
                Gtk::Label{")"},
        };

        Gtk::Popover _popover;
        Gtk::Grid _popoverGrid;
        Gtk::Label _xLabel{"X"}, _yLabel{"Y"}, _zLabel{"Z"};
        Gtk::SpinButton _xEntry, _yEntry, _zEntry;

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
