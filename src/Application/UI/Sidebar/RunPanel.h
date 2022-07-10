//
// Created by Jackson Campolattaro on 20/04/2022.
//

#ifndef N_BODY_RUNPANEL_H
#define N_BODY_RUNPANEL_H

#include <NBody/Simulation/Solver.h>

#include "Panel.h"
#include "../Widgets/View/FloatView.h"

#include <sigc++/sigc++.h>

#include <deque>

namespace UI {

    class RunPanel : public Panel {
    private:

        sigc::connection _idler;

        Gtk::Button *_stepButton;
        Gtk::ToggleButton *_runButton;
        Gtk::Label *_iterationsLabel;
        FloatView<5> *_stepTimeLabel;
        FloatView<10> *_averageStepTimeLabel;

        std::deque<std::chrono::duration<double>> _stepTimes;

        int _iterations = 0;

    public:

        RunPanel(NBody::Solver &solver);

    };

}


#endif //N_BODY_RUNPANEL_H
