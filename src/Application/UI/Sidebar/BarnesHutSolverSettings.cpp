//
// Created by Jackson Campolattaro on 7/21/22.
//

#include "BarnesHutSolverSettings.h"

UI::BarnesHutSolverSettings::BarnesHutSolverSettings(NBody::BarnesHutSolver<Gravity> &solver) :
        BuilderWidget<Gtk::Box>("/ui/barnes_hut_solver_settings.xml"),
        _solver(solver),
        _thetaEntry(getWidget<FloatEntry>("theta-float-entry")),
        _maxDepthEntry(getWidget<Gtk::SpinButton>("tree-depth-int-entry")),
        _maxLeafSizeEntry(getWidget<Gtk::SpinButton>("leaf-size-int-entry")) {

    signal_show().connect([&]() {
       spdlog::debug("Enabling Barnes-Hut Solver");
    });

    _thetaEntry.setValue(_solver.theta());
    _thetaEntry.signal_changed.connect([&](float v) {
        _solver.theta() = v;
    });

    _maxDepthEntry.set_range(1, std::numeric_limits<float>::max());
    _maxDepthEntry.set_increments(1.0, 10.0);
    _maxDepthEntry.set_value(_solver.maxDepth());
    _maxDepthEntry.signal_value_changed().connect([&]() {
        _solver.maxDepth() = _maxDepthEntry.get_value_as_int();
    });

    _maxLeafSizeEntry.set_range(1, std::numeric_limits<float>::max());
    _maxLeafSizeEntry.set_increments(1.0, 10.0);
    _maxLeafSizeEntry.set_value(_solver.maxLeafSize());
    _maxLeafSizeEntry.signal_value_changed().connect([&]() {
        _solver.maxLeafSize() = _maxLeafSizeEntry.get_value_as_int();
    });

}

UI::QuadrupoleBarnesHutSolverSettings::QuadrupoleBarnesHutSolverSettings(NBody::QuadrupoleBarnesHutSolver<Gravity> &solver) :
        BuilderWidget<Gtk::Box>("/ui/barnes_hut_solver_settings.xml"),
        _solver(solver),
        _thetaEntry(getWidget<FloatEntry>("theta-float-entry")),
        _maxDepthEntry(getWidget<Gtk::SpinButton>("tree-depth-int-entry")),
        _maxLeafSizeEntry(getWidget<Gtk::SpinButton>("leaf-size-int-entry")) {

    signal_show().connect([&]() {
        spdlog::debug("Enabling Barnes-Hut Solver");
    });

    _thetaEntry.setValue(_solver.theta());
    _thetaEntry.signal_changed.connect([&](float v) {
        _solver.theta() = v;
    });

    _maxDepthEntry.set_range(1, std::numeric_limits<float>::max());
    _maxDepthEntry.set_increments(1.0, 10.0);
    _maxDepthEntry.set_value(_solver.maxDepth());
    _maxDepthEntry.signal_value_changed().connect([&]() {
        _solver.maxDepth() = _maxDepthEntry.get_value_as_int();
    });

    _maxLeafSizeEntry.set_range(1, std::numeric_limits<float>::max());
    _maxLeafSizeEntry.set_increments(1.0, 10.0);
    _maxLeafSizeEntry.set_value(_solver.maxLeafSize());
    _maxLeafSizeEntry.signal_value_changed().connect([&]() {
        _solver.maxLeafSize() = _maxLeafSizeEntry.get_value_as_int();
    });

}
