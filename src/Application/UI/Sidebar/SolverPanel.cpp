//
// Created by jackcamp on 2/17/22.
//

#include "SolverPanel.h"

#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <adwaita.h>

UI::SolverPanel::SolverPanel(Gtk::Box::BaseObjectType *cobject,
                             const Glib::RefPtr<Gtk::Builder> &builder,
                             NBody::MultiSolver &multiSolver) :
        BuilderWidget<Gtk::Box>(cobject, builder, "/ui/solver_panel.xml"),
        _multiSolver(multiSolver),
        _naiveSolverSettings(multiSolver.getSolver<NBody::NaiveSolver>()),
        _barnesHutSolverSettings(multiSolver.getSolver<NBody::BarnesHutSolver>()),
        _solverDropdown(getWidget<Gtk::ListBoxRow>("solver-dropdown")),
        _solverStack(getWidget<Gtk::Stack>("stack")),
        _maxThreadCountEntry(getWidget<Gtk::SpinButton>("thread-count-int-entry")) {

    _solverStack.add(_naiveSolverSettings, "naive");
    _solverStack.add(_barnesHutSolverSettings, "barnes-hut");

    _multiSolver.select<NBody::NaiveSolver>();
    _solverStack.set_visible_child("naive");
    _solverDropdown.connect_property_changed("selected", [&]() {
        auto algorithm = _solverDropdown.get_property<guint>("selected");
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


    _maxThreadCountEntry.set_range(1, std::numeric_limits<float>::max());
    _maxThreadCountEntry.set_increments(1.0, 10.0);
    _maxThreadCountEntry.set_value(_multiSolver.maxThreadCount());
    _maxThreadCountEntry.signal_value_changed().connect([&](){
        _multiSolver.maxThreadCount() = _maxThreadCountEntry.get_value_as_int();
    });
}
