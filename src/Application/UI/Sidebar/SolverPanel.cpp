//
// Created by jackcamp on 2/17/22.
//

#include "SolverPanel.h"

#include <NBody/Simulation/Solvers/BarnesHutSolver.h>

UI::SolverPanel::SolverPanel(NBody::MultiSolver &multiSolver) :
        Panel("Solver"),
        _multiSolver(multiSolver),
        _builder(Gtk::Builder::create_from_resource("/ui/solver_panel.xml")),
        _placeholder(*_builder->get_widget<Gtk::Button>("placeholder")) {

    auto root = _builder->get_widget<Gtk::Widget>("root");
    _contents.append(*root);

    _multiSolver.select<NBody::NaiveSolver>();
    _placeholder.signal_clicked().connect([&] {
        if (_placeholder.get_label() == "Naive") {
            _placeholder.set_label("Barnes Hut");
            _multiSolver.select<NBody::BarnesHutSolver>();
        } else {
            _placeholder.set_label("Naive");
            _multiSolver.select<NBody::NaiveSolver>();
        }
    });
}
