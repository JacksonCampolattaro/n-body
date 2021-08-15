//
// Created by jackcamp on 8/15/21.
//

#ifndef N_BODY_PHYSICSPANE_H
#define N_BODY_PHYSICSPANE_H


#include <gtkmm/box.h>
#include <gtkmm/listbox.h>
#include <gtkmm/frame.h>
#include <gtkmm/button.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/scrolledwindow.h>

#include <NBody/Simulation/Simulation.h>

#include "Pane.h"
#include "../LabeledWidget.h"

using NBody::Simulation;

namespace Interface::Panes {

    class PhysicsPane : public Pane {
    private:

        Gtk::Box _box;

        Gtk::Label _label{"Physics"};
        Gtk::Frame _frame;
        Gtk::ListBox _listBox;

        LabeledWidget<Gtk::SpinButton> _gravity;
        LabeledWidget<Gtk::Button> _placeholder3;

    public:

        PhysicsPane();

    };

}

#endif //N_BODY_PHYSICSPANE_H
