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

    class PositionEntry : public Gtk::Button {
    private:

        Gtk::Box _previewBox;
        std::array<Gtk::Label, 7> _previewLabels {
            Gtk::Label{"("},
            Gtk::Label{"X"},
            Gtk::Label{", "},
            Gtk::Label{"Y"},
            Gtk::Label{", "},
            Gtk::Label{"Z"},
            Gtk::Label{")"},
        };

        Gtk::Label _currentValue;
        Gtk::Popover _popover;

        Gtk::Grid _popoverGrid;
        Gtk::Label _xLabel{"X"}, _yLabel{"Y"}, _zLabel{"Z"};
        Gtk::SpinButton _xEntry, _yEntry, _zEntry;


    public:

        PositionEntry(double x, double y, double z);

        void on_valueChanged();
    };

}

#endif //N_BODY_POSITIONENTRY_H