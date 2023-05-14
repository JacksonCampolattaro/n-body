//
// Created by Jackson Campolattaro on 1/10/23.
//

#include "LinearBVHSolverSettings.h"

UI::LinearBVHSolverSettings::LinearBVHSolverSettings(NBody::LinearBVHSolver<Gravity> &solver) :
        BuilderWidget<Gtk::Box>("/ui/linear_bvh_solver_settings.xml"),
        _solver(solver),
        _thetaEntry(getWidget<FloatEntry>("theta-float-entry")) {

    _thetaEntry.setValue(_solver.theta());
    _thetaEntry.signal_changed.connect([&](float v) {
        _solver.theta() = v;
    });

}

UI::QuadrupoleLinearBVHSolverSettings::QuadrupoleLinearBVHSolverSettings(NBody::QuadrupoleLinearBVHSolver<Gravity> &solver) :
        BuilderWidget<Gtk::Box>("/ui/linear_bvh_solver_settings.xml"),
        _solver(solver),
        _thetaEntry(getWidget<FloatEntry>("theta-float-entry")) {

    _thetaEntry.setValue(_solver.theta());
    _thetaEntry.signal_changed.connect([&](float v) {
        _solver.theta() = v;
    });
}
