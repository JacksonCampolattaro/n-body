//
// Created by jackcamp on 2/17/22.
//

#ifndef N_BODY_SOLVERPANEL_H
#define N_BODY_SOLVERPANEL_H

#include "Application/UI/Widgets/ListView/StackView.h"
#include "Application/UI/Widgets/ListView/DropDownView.h"

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/MultiSolver.h>

#include <gtkmm/button.h>
#include <gtkmm/stack.h>
#include <gtkmm/stackswitcher.h>
#include <gtkmm/dropdown.h>
#include <gtkmm/listboxrow.h>

#include <gtkmm/singleselection.h>
#include <gtkmm/label.h>

#include "NaiveSolverSettings.h"
#include "BarnesHutSolverSettings.h"
#include "LinearBVHSolverSettings.h"
#include "MVDRSolverSettings.h"
#include "OctreeDualTraversalSolverSettings.h"

#include "Application/UI/Widgets/BuilderWidget.h"

namespace UI {

    class SolverPanel : public BuilderWidget<Gtk::Box> {
    private:

        NBody::MultiSolver &_multiSolver;
        Gtk::SpinButton &_maxThreadCountEntry;
        PositiveFloatEntry &_timeStepEntry;

        DropDownView &_solverDropdown;
        StackView &_solverStack;

    public:

        SolverPanel(Gtk::Box::BaseObjectType *cobject,
                    const Glib::RefPtr<Gtk::Builder> &builder,
                    NBody::MultiSolver &multiSolver);

    };

}


#endif //N_BODY_SOLVERPANEL_H
