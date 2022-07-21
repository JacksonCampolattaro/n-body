//
// Created by Jackson Campolattaro on 7/21/22.
//

#ifndef N_BODY_NAIVESOLVERSETTINGS_H
#define N_BODY_NAIVESOLVERSETTINGS_H

#include <NBody/Simulation/Solvers/NaiveSolver.h>

#include <gtkmm/box.h>
#include <gtkmm/builder.h>

namespace UI {

    class NaiveSolverSettings : public Gtk::Box {
    private:

        NBody::NaiveSolver &_solver;

        Glib::RefPtr<Gtk::Builder> _builder;

    public:

        NaiveSolverSettings(NBody::NaiveSolver &solver);

    };

}

#endif //N_BODY_NAIVESOLVERSETTINGS_H
