//
// Created by Jackson Campolattaro on 1/7/23.
//

#ifndef N_BODY_CONTINUOUSRUNNERCONTROLLER_H
#define N_BODY_CONTINUOUSRUNNERCONTROLLER_H

#include <gtkmm/togglebutton.h>
#include <gtkmm/stack.h>

#include <NBody/Simulation/Runners/ContinuousRunner.h>

#include "../Widgets/BuilderWidget.h"

namespace UI {

    class ContinuousRunnerController : public BuilderWidget<Gtk::Box> {
    private:

        NBody::ContinuousRunner &_runner;

        Gtk::ToggleButton &_runButton;

    public:

        explicit ContinuousRunnerController(NBody::ContinuousRunner &runner);

    };

}

#endif //N_BODY_CONTINUOUSRUNNERCONTROLLER_H
