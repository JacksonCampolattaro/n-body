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
        _solverDropdown(getWidget<Gtk::DropDown>("solver-dropdown")),
        _solverStack(getWidget<Gtk::Stack>("stack")),
        _maxThreadCountEntry(getWidget<Gtk::SpinButton>("thread-count-int-entry")),
        _timeStepEntry(getWidget<PositiveFloatEntry>("time-step-float-entry")) {

    _timeStepEntry.setValue(NBody::Solver::timeStep());
    _timeStepEntry.signal_changed.connect([&](float t) {
       NBody::Solver::timeStep() = t;
    });

    _maxThreadCountEntry.set_range(1, std::numeric_limits<float>::max());
    _maxThreadCountEntry.set_increments(1.0, 10.0);
    _maxThreadCountEntry.set_value(_multiSolver.maxThreadCount());
    _maxThreadCountEntry.signal_value_changed().connect([&]() {
        _multiSolver.maxThreadCount() = _maxThreadCountEntry.get_value_as_int();
    });

    _solverStack.add(_naiveSolverSettings, "naive", "Naive");
    _solverStack.add(_barnesHutSolverSettings, "barnes-hut", "Barnes-Hut");
    _solverStack.add(_placeholderSolverSettings, "placeholder", "(Placeholder)");

    auto pagesList = std::dynamic_pointer_cast<Gio::ListModel>(_solverStack.get_pages());
    assert(pagesList);
    _solverDropdown.set_expression(Gtk::PropertyExpression<Glib::ustring>::create(
            pagesList->get_item_type(),
            "title")
    );
    _solverDropdown.set_model(pagesList);
    _solverDropdown.property_selected().signal_changed().connect([&]() {

        auto name = std::dynamic_pointer_cast<Gtk::StackPage>(_solverDropdown.get_selected_item())->get_name();
        _solverStack.set_visible_child(name);

        if (name == "naive")
            _multiSolver.select<NBody::NaiveSolver>();
        else if (name == "barnes-hut")
            _multiSolver.select<NBody::BarnesHutSolver>();
        else
            spdlog::error("Unrecognized solver selected");

    });
    _multiSolver.select<NBody::NaiveSolver>();
}
