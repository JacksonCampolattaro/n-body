//
// Created by Jackson Campolattaro on 1/17/23.
//

#include "MVDRSolverSettings.h"

UI::MVDRSolverSettings::MVDRSolverSettings(NBody::MVDRSolver<Gravity> &solver) :
        BuilderWidget<Gtk::Box>("/ui/mvdr_solver_settings.xml"),
        _solver(solver),
        _thetaEntry(getWidget<FloatEntry>("theta-float-entry")),
        _maxPassiveTreeDepthEntry(getWidget<Gtk::SpinButton>("passive-tree-depth-int-entry")),
        _maxPassiveTreeLeafSizeEntry(getWidget<Gtk::SpinButton>("passive-tree-leaf-size-int-entry")) {

    signal_show().connect([&]() {
        spdlog::debug("Enabling MVDR Solver");
    });

    _thetaEntry.setValue(_solver.theta());
    _thetaEntry.signal_changed.connect([&](float v) {
        _solver.theta() = v;
    });

    _maxPassiveTreeDepthEntry.set_range(1, std::numeric_limits<float>::max());
    _maxPassiveTreeDepthEntry.set_increments(1.0, 10.0);
    _maxPassiveTreeDepthEntry.set_value(_solver.passiveTreeMaxDepth());
    _maxPassiveTreeDepthEntry.signal_value_changed().connect([&]() {
        _solver.passiveTreeMaxDepth() = _maxPassiveTreeDepthEntry.get_value_as_int();
    });

    _maxPassiveTreeLeafSizeEntry.set_range(1, std::numeric_limits<float>::max());
    _maxPassiveTreeLeafSizeEntry.set_increments(1.0, 10.0);
    _maxPassiveTreeLeafSizeEntry.set_value(_solver.passiveTreeMaxLeafSize());
    _maxPassiveTreeLeafSizeEntry.signal_value_changed().connect([&]() {
        _solver.passiveTreeMaxLeafSize() = _maxPassiveTreeLeafSizeEntry.get_value_as_int();
    });
}

UI::QuadrupoleMVDRSolverSettings::QuadrupoleMVDRSolverSettings(NBody::QuadrupoleMVDRSolver<Gravity> &solver) :
        BuilderWidget<Gtk::Box>("/ui/mvdr_solver_settings.xml"),
        _solver(solver),
        _thetaEntry(getWidget<FloatEntry>("theta-float-entry")),
        _maxPassiveTreeDepthEntry(getWidget<Gtk::SpinButton>("passive-tree-depth-int-entry")),
        _maxPassiveTreeLeafSizeEntry(getWidget<Gtk::SpinButton>("passive-tree-leaf-size-int-entry")) {

    signal_show().connect([&]() {
        spdlog::debug("Enabling MVDR Solver");
    });

    _thetaEntry.setValue(_solver.theta());
    _thetaEntry.signal_changed.connect([&](float v) {
        _solver.theta() = v;
    });

    _maxPassiveTreeDepthEntry.set_range(1, std::numeric_limits<float>::max());
    _maxPassiveTreeDepthEntry.set_increments(1.0, 10.0);
    _maxPassiveTreeDepthEntry.set_value(_solver.passiveTreeMaxDepth());
    _maxPassiveTreeDepthEntry.signal_value_changed().connect([&]() {
        _solver.passiveTreeMaxDepth() = _maxPassiveTreeDepthEntry.get_value_as_int();
    });

    _maxPassiveTreeLeafSizeEntry.set_range(1, std::numeric_limits<float>::max());
    _maxPassiveTreeLeafSizeEntry.set_increments(1.0, 10.0);
    _maxPassiveTreeLeafSizeEntry.set_value(_solver.passiveTreeMaxLeafSize());
    _maxPassiveTreeLeafSizeEntry.signal_value_changed().connect([&]() {
        _solver.passiveTreeMaxLeafSize() = _maxPassiveTreeLeafSizeEntry.get_value_as_int();
    });
}
