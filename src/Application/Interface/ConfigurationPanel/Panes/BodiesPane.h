#ifndef N_BODY_BODIESPANE_H
#define N_BODY_BODIESPANE_H

#include <gtkmm/box.h>
#include <gtkmm/listbox.h>
#include <gtkmm/frame.h>
#include <gtkmm/button.h>
#include <gtkmm/separator.h>
#include <gtkmm/scrolledwindow.h>

#include <NBody/Simulation/Simulation.h>

#include "Pane.h"
#include "../LabeledWidget.h"

using NBody::Simulation::Simulation;

namespace Interface::Panes {

    class BodiesPane : public Pane {
    private:

        Simulation &_simulation;

        Gtk::Box _box;

        Gtk::Label _label{"Bodies"};
        Gtk::Frame _frame;
        Gtk::ListBox _listBox;

        LabeledWidget<Gtk::Label> _bodyCount;
        Gtk::Separator _separator;
        LabeledWidget<Gtk::Button> _placeholder3;

    public:

        BodiesPane(Simulation &simulation);

    };
}

#endif //N_BODY_BODIESPANE_H
