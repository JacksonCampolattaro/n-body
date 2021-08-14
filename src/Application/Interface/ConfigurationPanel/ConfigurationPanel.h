//
// Created by jackcamp on 8/13/21.
//

#ifndef N_BODY_CONFIGURATIONPANEL_H
#define N_BODY_CONFIGURATIONPANEL_H

#include <gtkmm/entry.h>
#include <gtkmm/box.h>
#include <gtkmm/notebook.h>

#include "Panes/BodiesPane.h"

#include <NBody/Simulation/Simulation.h>

using NBody::Simulation::Simulation;

namespace Interface {

    class ConfigurationPanel : public Gtk::Notebook {
    private:

        Gtk::Image
                _bodiesIcon,
                _physicsIcon,
                _solverIcon,
                _positionIcon,
                _recordingIcon,
                _runIcon;

        Gtk::Label
                _bodiesPlaceholder{"bodies"},
                _physicsPlaceholder{"physics"},
                _solverPlaceholder{"solver"},
                _positionPlaceholder{"position"},
                _recordingPlaceholder{"recording"},
                _runPlaceholder{"Some long text, to make the window wide"};

        Panes::BodiesPane _bodiesPane;

    public:

        ConfigurationPanel(Simulation &simulation);

    public:
        // Signals

    public:
        // Slots

    };
}


#endif //N_BODY_CONFIGURATIONPANEL_H
