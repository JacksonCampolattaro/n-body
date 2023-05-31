//
// Created by Jackson Campolattaro on 7/21/22.
//

#include <gtkmm/label.h>
#include "NaiveSolverSettings.h"

UI::NaiveSolverSettings::NaiveSolverSettings(NBody::NaiveSolver<Gravity> &solver) :
        Gtk::Box(Gtk::Orientation::VERTICAL),
        _solver(solver),
        _builder(Gtk::Builder::create_from_resource("/ui/naive_solver_settings.xml")) {

    auto root = _builder->get_widget<Gtk::Widget>("root");
    append(*root);

}
