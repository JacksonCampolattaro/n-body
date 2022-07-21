//
// Created by Jackson Campolattaro on 7/21/22.
//

#include "BarnesHutSolverSettings.h"

UI::BarnesHutSolverSettings::BarnesHutSolverSettings(NBody::BarnesHutSolver &solver) :
        Gtk::Box(Gtk::Orientation::VERTICAL),
        _solver(solver),
        _builder(Gtk::Builder::create_from_resource("/ui/barnes_hut_solver_settings.xml")) {

    auto root = _builder->get_widget<Gtk::Widget>("root");
    append(*root);

}
