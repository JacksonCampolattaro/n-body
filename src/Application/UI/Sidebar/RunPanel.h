//
// Created by Jackson Campolattaro on 20/04/2022.
//

#ifndef N_BODY_RUNPANEL_H
#define N_BODY_RUNPANEL_H

#include "Panel.h"
#include "../Widgets/View/FloatView.h"

#include <NBody/Simulation/Solver.h>

#include <gtkmm/listboxrow.h>

#include <sigc++/sigc++.h>

#include <deque>

namespace UI {

    class RunPanel : public Panel {
    private:

        NBody::Solver &_solver;
        sigc::connection _idler;

        Glib::RefPtr<Gtk::Builder> _builder;

        Gtk::ListBoxRow &_runModeDropdown;

        Gtk::ToggleButton &_runButton;

        Gtk::Label &_iterationsLabel;
        PreciseFloatView &_stepTimeLabel;
        PreciseFloatView &_averageStepTimeLabel;

        std::deque<std::chrono::duration<double>> _stepTimes;

        std::size_t _iterations = 0;
        std::size_t _currentIterations = 0;

    public:

        RunPanel(NBody::Solver &solver);

        void run();
        void run(std::size_t n);
        void stop();

    };

}


#endif //N_BODY_RUNPANEL_H
