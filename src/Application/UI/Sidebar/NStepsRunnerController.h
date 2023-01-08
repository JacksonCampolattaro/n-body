//
// Created by Jackson Campolattaro on 1/7/23.
//

#ifndef N_BODY_NSTEPSRUNNERCONTROLLER_H
#define N_BODY_NSTEPSRUNNERCONTROLLER_H

#include <gtkmm/entry.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/stack.h>

#include <NBody/Simulation/Runners/NStepsRunner.h>

#include "../Widgets/BuilderWidget.h"

namespace UI {

    class NStepsRunnerController : public BuilderWidget<Gtk::Box> {
    private:

        NBody::NStepsRunner &_runner;

        Gtk::Entry &_stepCountEntry;
        Gtk::ToggleButton &_runButton;

    public:

        explicit NStepsRunnerController(NBody::NStepsRunner &runner);

    };

}

#endif //N_BODY_NSTEPSRUNNERCONTROLLER_H
