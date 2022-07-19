//
// Created by jackcamp on 2/17/22.
//

#ifndef N_BODY_SOLVERPANEL_H
#define N_BODY_SOLVERPANEL_H

#include <NBody/Simulation/Simulation.h>

#include <gtkmm/button.h>
#include <gtkmm/stack.h>
#include <gtkmm/stackswitcher.h>
#include <gtkmm/dropdown.h>
#include <gtkmm/listboxrow.h>

#include "Panel.h"
#include "../../MultiSolver.h"

namespace UI {

    class SolverPanel : public Panel {
    private:

        NBody::MultiSolver &_multiSolver;

        Glib::RefPtr<Gtk::Builder> _builder;

        Gtk::ListBoxRow &_solverDropdown;
        Gtk::Stack &_solverStack;

    public:

        SolverPanel(NBody::MultiSolver &multiSolver);

    };

}


#endif //N_BODY_SOLVERPANEL_H
