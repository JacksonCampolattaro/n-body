//
// Created by Jackson Campolattaro on 20/04/2022.
//

#ifndef N_BODY_RUNPANEL_H
#define N_BODY_RUNPANEL_H

#include <deque>

#include <sigc++/sigc++.h>

#include <gtkmm/listboxrow.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/stack.h>
#include <gtkmm/entry.h>

#include <NBody/Simulation/Solver.h>

#include "Application/UI/Widgets/BuilderWidget.h"
#include "Application/UI/Widgets/View/FloatView.h"
#include "Application/UI/Widgets/View/TimeView.h"

namespace UI {

    class RunPanel : public BuilderWidget<Gtk::Box> {
    private:

        NBody::Solver &_solver;
        sigc::connection _idler;

        Gtk::ListBoxRow &_runModeDropdown;

        Gtk::Label &_statusLabel;

        Gtk::ToggleButton &_runContinuouslyButton;
        Gtk::ToggleButton &_runOneStepButton;
        Gtk::ToggleButton &_runNStepsButton;
        Gtk::Entry &_stepCountEntry;

        Gtk::Stack &_runControllerStack;

        Gtk::Label &_iterationsLabel;
        PreciseFloatView &_stepTimeLabel;
        PreciseFloatView &_averageStepTimeLabel;
        TimeView &_computeTimeView;
        Gtk::Box &_remainingTime;
        TimeView &_remainingTimeView;

        std::deque<std::chrono::duration<double>> _stepTimes;
        std::chrono::duration<double> _totalElapsedComputeTime = {};

        std::size_t _iterations = 0;
        std::size_t _currentIterations = 0;

    public:

        RunPanel(Gtk::Box::BaseObjectType *cobject,
                 const Glib::RefPtr<Gtk::Builder> &builder,
                 NBody::Solver &solver);

        void run(std::size_t n = 1);

        void stop();

    };

}


#endif //N_BODY_RUNPANEL_H
