//
// Created by Jackson Campolattaro on 20/04/2022.
//

#ifndef N_BODY_RUNPANEL_H
#define N_BODY_RUNPANEL_H

#include "ContinuousRunnerController.h"
#include "OneStepRunnerController.h"
#include "NStepsRunnerController.h"

#include "Application/UI/Widgets/BuilderWidget.h"
#include "Application/UI/Widgets/View/FloatView.h"
#include "Application/UI/Widgets/View/TimeView.h"
#include "Application/UI/Widgets/ListView/DropDownView.h"
#include "Application/UI/Widgets/ListView/StackView.h"

#include <NBody/Simulation/Solver.h>
#include <NBody/Simulation/Runners/MultiRunner.h>

#include <gtkmm/listboxrow.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/stack.h>
#include <gtkmm/entry.h>

#include <sigc++/sigc++.h>

#include <deque>

namespace UI {

    class RunPanel : public BuilderWidget<Gtk::Box> {
    private:

        NBody::Solver &_solver;
        sigc::connection _idler;

        NBody::MultiRunner _multiRunner;
        DropDownView &_runnerDropdown;
        StackView &_runnerStack;

        Gtk::Label &_statusLabel;

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
                 NBody::Solver &solver, NBody::MultiRunner &runner);

    };

}


#endif //N_BODY_RUNPANEL_H
