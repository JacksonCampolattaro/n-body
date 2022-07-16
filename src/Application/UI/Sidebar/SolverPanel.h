//
// Created by jackcamp on 2/17/22.
//

#ifndef N_BODY_SOLVERPANEL_H
#define N_BODY_SOLVERPANEL_H

#include <NBody/Simulation/Simulation.h>

#include <gtkmm/button.h>

#include "Panel.h"
#include "../../MultiSolver.h"

namespace UI {

    class SolverPanel : public Panel {
    private:

        NBody::MultiSolver &_multiSolver;

        Glib::RefPtr<Gtk::Builder> _builder;

        Gtk::Button &_placeholder;

    public:

        SolverPanel(NBody::MultiSolver &multiSolver);

    };

}


#endif //N_BODY_SOLVERPANEL_H
