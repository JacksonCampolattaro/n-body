//
// Created by Jackson Campolattaro on 1/7/23.
//

#ifndef N_BODY_ONESTEPRUNNERCONTROLLER_H
#define N_BODY_ONESTEPRUNNERCONTROLLER_H

#include <gtkmm/togglebutton.h>

#include <NBody/Simulation/Runners/OneStepRunner.h>

#include "../Widgets/BuilderWidget.h"

namespace UI {

class OneStepRunnerController : public BuilderWidget<Gtk::Box> {
    private:

        NBody::OneStepRunner<Gravity> &_runner;

        Gtk::ToggleButton &_stepButton;

    public:

        explicit OneStepRunnerController(NBody::OneStepRunner<Gravity> &runner);

    };

}


#endif //N_BODY_ONESTEPRUNNERCONTROLLER_H
