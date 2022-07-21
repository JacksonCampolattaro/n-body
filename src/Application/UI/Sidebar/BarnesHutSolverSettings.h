//
// Created by Jackson Campolattaro on 7/21/22.
//

#ifndef N_BODY_BARNESHUTSOLVERSETTINGS_H
#define N_BODY_BARNESHUTSOLVERSETTINGS_H

#include <NBody/Simulation/Solvers/BarnesHutSolver.h>

#include <gtkmm/box.h>
#include <gtkmm/builder.h>

namespace UI {

    class BarnesHutSolverSettings : public Gtk::Box {
    private:

        NBody::BarnesHutSolver &_solver;

        Glib::RefPtr<Gtk::Builder> _builder;

    public:

        BarnesHutSolverSettings(NBody::BarnesHutSolver &solver);

    };

}

#endif //N_BODY_BARNESHUTSOLVERSETTINGS_H
