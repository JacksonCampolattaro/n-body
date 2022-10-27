//
// Created by jackcamp on 2/17/22.
//

#ifndef N_BODY_SOLVERPANEL_H
#define N_BODY_SOLVERPANEL_H

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

#include "Application/UI/Widgets/BuilderWidget.h"

namespace UI {

    class SolverPanel : public BuilderWidget<Gtk::Box> {
    private:

        NBody::MultiSolver &_multiSolver;

        NaiveSolverSettings _naiveSolverSettings;
        BarnesHutSolverSettings _barnesHutSolverSettings;
        Gtk::Label _placeholderSolverSettings{"(This is a Placeholder)"};

        Gtk::DropDown &_solverDropdown;
        Gtk::Stack &_solverStack;
        Gtk::SpinButton &_maxThreadCountEntry;

    public:

        SolverPanel(Gtk::Box::BaseObjectType *cobject,
                    const Glib::RefPtr<Gtk::Builder> &builder,
                    NBody::MultiSolver &multiSolver);

    };

}


#endif //N_BODY_SOLVERPANEL_H
