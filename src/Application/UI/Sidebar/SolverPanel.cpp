//
// Created by jackcamp on 2/17/22.
//

#include "SolverPanel.h"

#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <adwaita.h>

UI::SolverPanel::SolverPanel(NBody::MultiSolver &multiSolver) :
        Panel("Solver"),
        _multiSolver(multiSolver),
        _builder(Gtk::Builder::create_from_resource("/ui/solver_panel.xml")),
        _solverDropdown(*_builder->get_widget<Gtk::ListBoxRow>("solver-dropdown")),
        _solverStack(*_builder->get_widget<Gtk::Stack>("stack")) {

    auto root = _builder->get_widget<Gtk::Widget>("root");
    _contents.append(*root);

    _multiSolver.select<NBody::NaiveSolver>();
    _solverStack.set_visible_child("naive");
    _solverDropdown.connect_property_changed("selected", [&]() {
        auto algorithm = _solverDropdown.get_property<guint>("selected");
        _solverDropdown.set_property("selected", algorithm);
        switch (algorithm) {
            case 0:
                _multiSolver.select<NBody::NaiveSolver>();
                _solverStack.set_visible_child("naive");
                break;
            case 1:
                _multiSolver.select<NBody::BarnesHutSolver>();
                _solverStack.set_visible_child("barnes-hut");
                break;
            default:
                spdlog::error("Unrecognized algorithm selected");
                break;
        }
    });
}
