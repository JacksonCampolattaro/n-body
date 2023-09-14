//
// Created by jackcamp on 2/17/22.
//

#include "SolverPanel.h"

#include <adwaita.h>

UI::SolverPanel::SolverPanel(Gtk::Box::BaseObjectType *cobject,
                             const Glib::RefPtr<Gtk::Builder> &builder,
                             NBody::MultiSolver<Gravity> &multiSolver) :
        BuilderWidget<Gtk::Box>(cobject, builder, "/ui/solver_panel.xml"),
        _multiSolver(multiSolver),
        _solverDropdown(getWidget<DropDownView>("solver-dropdown")),
        _solverStack(getWidget<StackView>("stack")),
        _maxThreadCountEntry(getWidget<Gtk::SpinButton>("thread-count-int-entry")),
        _timeStepEntry(getWidget<PositiveFloatEntry>("time-step-float-entry")) {

    _timeStepEntry.set_digits(4);
    _timeStepEntry.set_increments(0.0001, 0.001);
    _timeStepEntry.setValue(NBody::Solver<Gravity>::timeStep());
    _timeStepEntry.signal_changed.connect([&](float t) {
        NBody::Solver<Gravity>::timeStep() = t;
    });

    _maxThreadCountEntry.set_range(1, std::numeric_limits<float>::max());
    _maxThreadCountEntry.set_increments(1.0, 10.0);
    _maxThreadCountEntry.set_value(_multiSolver.maxThreadCount());
    _maxThreadCountEntry.signal_value_changed().connect([&]() {
        _multiSolver.maxThreadCount() = _maxThreadCountEntry.get_value_as_int();
    });

    _solverDropdown.set_expression(Gtk::ClosureExpression<Glib::ustring>::create(
            [&](const Glib::RefPtr<Glib::ObjectBase> &item) {
                return std::dynamic_pointer_cast<NBody::Solver<Gravity>>(item)->name();
            }
    ));
    _solverDropdown.set_model(_multiSolver.selectionModel());

    _solverStack.set_expression(Gtk::ClosureExpression<Gtk::Widget *>::create(
            [&](const Glib::RefPtr<Glib::ObjectBase> &item) -> Gtk::Widget * {
                auto solver = std::dynamic_pointer_cast<NBody::Solver<Gravity>>(item);
                if (solver->id() == "naive")
                    return new SolverSettings((NBody::NaiveSolver<Gravity> &) *solver);
                if (solver->id() == "barnes-hut")
                    return new SolverSettings((BarnesHutSolver<Gravity> &) *solver);
                if (solver->id() == "barnes-hut-4p")
                    return new SolverSettings((QuadrupoleBarnesHutSolver<Gravity> &) *solver);
                if (solver->id() == "barnes-hut-8p")
                    return new SolverSettings((OctupoleBarnesHutSolver<Gravity> &) *solver);
                if (solver->id() == "linear-bvh")
                    return new SolverSettings((LinearBVHSolver<Gravity> &) *solver);
                if (solver->id() == "linear-bvh-4p")
                    return new SolverSettings((QuadrupoleLinearBVHSolver<Gravity> &) *solver);
                if (solver->id() == "linear-bvh-8p")
                    return new SolverSettings((OctupoleLinearBVHSolver<Gravity> &) *solver);
                if (solver->id() == "implicit-mvdr")
                    return new SolverSettings((ImplicitMVDRSolver<Gravity> &) *solver);
                if (solver->id() == "implicit-mvdr-4p")
                    return new SolverSettings((QuadrupoleImplicitMVDRSolver<Gravity> &) *solver);
                if (solver->id() == "implicit-mvdr-8p")
                    return new SolverSettings((OctupoleImplicitMVDRSolver<Gravity> &) *solver);
                if (solver->id() == "implicit-fmm-4p")
                    return new SolverSettings((QuadrupoleImplicitFMMSolver<Gravity> &) *solver);
                if (solver->id() == "implicit-fmm-8p")
                    return new SolverSettings((OctupoleImplicitFMMSolver<Gravity> &) *solver);
                return new Gtk::Label{"Unrecognized Solver"};
            }
    ));
    _solverStack.set_model(_multiSolver.selectionModel());

}
