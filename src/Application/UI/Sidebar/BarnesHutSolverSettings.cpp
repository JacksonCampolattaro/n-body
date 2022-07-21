//
// Created by Jackson Campolattaro on 7/21/22.
//

#include "BarnesHutSolverSettings.h"

UI::BarnesHutSolverSettings::BarnesHutSolverSettings(NBody::BarnesHutSolver &solver) :
        Gtk::Box(Gtk::Orientation::VERTICAL),
        _solver(solver),
        _builder(Gtk::Builder::create_from_resource("/ui/barnes_hut_solver_settings.xml")),
        _thetaEntry(*Gtk::Builder::get_widget_derived<FloatEntry>(_builder, "theta-float-entry")),
        _maxDepthEntry(*_builder->get_widget<Gtk::SpinButton>("tree-depth-int-entry")) {

    auto root = _builder->get_widget<Gtk::Widget>("root");
    append(*root);

    _thetaEntry.setValue(_solver.theta());
    _thetaEntry.signal_changed.connect([&](float v) {
        _solver.theta() = v;
    });

    _maxDepthEntry.set_range(1, std::numeric_limits<float>::max());
    _maxDepthEntry.set_increments(1.0, 10.0);
    _maxDepthEntry.set_value(_solver.maxDepth());
    _maxDepthEntry.signal_value_changed().connect([&](){
       _solver.maxDepth() = _maxDepthEntry.get_value_as_int();
    });

}
