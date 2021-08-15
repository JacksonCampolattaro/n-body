//
// Created by jackcamp on 8/13/21.
//

#ifndef N_BODY_CONFIGURATIONPANEL_H
#define N_BODY_CONFIGURATIONPANEL_H

#include <gtkmm/entry.h>
#include <gtkmm/box.h>
#include <gtkmm/notebook.h>

#include "Panes/BodiesPane.h"
#include "Panes/PhysicsPane.h"

#include <NBody/Simulation/Simulation.h>

using NBody::Simulation;

namespace Interface {

    class ConfigurationPanel : public Gtk::Notebook {
    private:

        Gtk::Image
                _solverIcon,
                _positionIcon,
                _recordingIcon,
                _runIcon;

        Gtk::Label
                _solverPlaceholder{"solver"},
                _positionPlaceholder{"position"},
                _recordingPlaceholder{"recording"},
                _runPlaceholder{"Some long text, to make the window wide"};

        Panes::BodiesPane _bodiesPane;
        Panes::PhysicsPane _physicsPane;


    public:

        ConfigurationPanel(Simulation &simulation);

    public:
        // Signals

    public:
        // Slots

    };
}


#endif //N_BODY_CONFIGURATIONPANEL_H
