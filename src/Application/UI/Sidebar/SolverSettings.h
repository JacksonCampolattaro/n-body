//
// Created by Jackson Campolattaro on 6/3/23.
//

#ifndef N_BODY_SOLVERSETTINGS_H
#define N_BODY_SOLVERSETTINGS_H

#include "NBody/Simulation/Solvers/FMMSolver.h"

#include "Application/UI/Widgets/BuilderWidget.h"
#include "Application/UI/Widgets/Entry/FloatEntry.h"

namespace UI {

    template<typename SolverType>
    class SolverSettings : public BuilderWidget<Gtk::Box> {
    private:

        SolverType &_solver;

        // Depending on the solver, some of these will be hidden
        FloatEntry &_thetaEntry;
        Gtk::SpinButton &_maxDepthEntry;
        Gtk::SpinButton &_maxLeafSizeEntry;

        Gtk::SpinButton &_maxActiveDepthEntry;
        Gtk::SpinButton &_maxActiveLeafSizeEntry;

        Gtk::SpinButton &_maxPassiveDepthEntry;
        Gtk::SpinButton &_maxPassiveLeafSizeEntry;

    public:

        SolverSettings(SolverType &solver) :
                BuilderWidget<Gtk::Box>("/ui/solver_settings.xml"),
                _solver(solver),
                _thetaEntry(getWidget<FloatEntry>("theta-float-entry")),
                _maxDepthEntry(getWidget<Gtk::SpinButton>("tree-depth-int-entry")),
                _maxLeafSizeEntry(getWidget<Gtk::SpinButton>("tree-leaf-size-int-entry")),
                _maxActiveDepthEntry(getWidget<Gtk::SpinButton>("active-tree-depth-int-entry")),
                _maxActiveLeafSizeEntry(getWidget<Gtk::SpinButton>("active-tree-leaf-size-int-entry")),
                _maxPassiveDepthEntry(getWidget<Gtk::SpinButton>("passive-tree-depth-int-entry")),
                _maxPassiveLeafSizeEntry(getWidget<Gtk::SpinButton>("passive-tree-leaf-size-int-entry")) {

            signal_show().connect([&]() {
                spdlog::debug("Enabling {} Solver", _solver.name());
            });

            // todo: use constexpr to make this work for any solver type

            if constexpr (requires{ _solver.descentCriterion().theta(); }) {
                _thetaEntry.setValue(_solver.descentCriterion().theta());
                _thetaEntry.signal_changed.connect([&](float v) {
                    _solver.descentCriterion().theta() = v;
                });
            } else getWidget<Gtk::Widget>("theta").hide();


            if constexpr (requires{ _solver.tree().maxDepth(); }) {
                _maxDepthEntry.set_range(1, std::numeric_limits<float>::max());
                _maxDepthEntry.set_increments(1.0, 10.0);
                _maxDepthEntry.set_value(_solver.tree().maxDepth());
                _maxDepthEntry.signal_value_changed().connect([&]() {
                    _solver.tree().maxDepth() = _maxDepthEntry.get_value_as_int();
                });
            } else getWidget<Gtk::Widget>("tree-depth").hide();

            if constexpr (requires{ _solver.tree().maxLeafSize(); }) {
                _maxLeafSizeEntry.set_range(1, std::numeric_limits<float>::max());
                _maxLeafSizeEntry.set_increments(1.0, 10.0);
                _maxLeafSizeEntry.set_value(_solver.tree().maxLeafSize());
                _maxLeafSizeEntry.signal_value_changed().connect([&]() {
                    _solver.tree().maxLeafSize() = _maxLeafSizeEntry.get_value_as_int();
                });
            } else getWidget<Gtk::Widget>("tree-leaf-size").hide();

            if constexpr (requires{ _solver.activeTree(); }) {

                if constexpr (requires{ _solver.activeTree().maxDepth(); }) {
                    _maxActiveDepthEntry.set_range(1, std::numeric_limits<float>::max());
                    _maxActiveDepthEntry.set_increments(1.0, 10.0);
                    _maxActiveDepthEntry.set_value(_solver.activeTree().maxDepth());
                    _maxActiveDepthEntry.signal_value_changed().connect([&]() {
                        _solver.activeTree().maxDepth() = _maxActiveDepthEntry.get_value_as_int();
                    });
                } else getWidget<Gtk::Widget>("active-tree-depth").hide();

                if constexpr (requires{ _solver.activeTree().maxLeafSize(); }) {
                    _maxActiveLeafSizeEntry.set_range(1, std::numeric_limits<float>::max());
                    _maxActiveLeafSizeEntry.set_increments(1.0, 10.0);
                    _maxActiveLeafSizeEntry.set_value(_solver.activeTree().maxLeafSize());
                    _maxActiveLeafSizeEntry.signal_value_changed().connect([&]() {
                        _solver.activeTree().maxLeafSize() = _maxActiveLeafSizeEntry.get_value_as_int();
                    });
                } else getWidget<Gtk::Widget>("active-tree-leaf-size").hide();

            } else getWidget<Gtk::Widget>("active-tree").hide();

            if constexpr (requires{ _solver.passiveTree(); }) {

                if constexpr (requires{ _solver.passiveTree().maxDepth(); }) {
                    _maxPassiveDepthEntry.set_range(1, std::numeric_limits<float>::max());
                    _maxPassiveDepthEntry.set_increments(1.0, 10.0);
                    _maxPassiveDepthEntry.set_value(_solver.passiveTree().maxDepth());
                    _maxPassiveDepthEntry.signal_value_changed().connect([&]() {
                        _solver.passiveTree().maxDepth() = _maxPassiveDepthEntry.get_value_as_int();
                    });
                } else getWidget<Gtk::Widget>("passive-tree-depth").hide();

                if constexpr (requires{ _solver.passiveTree().maxLeafSize(); }) {
                    _maxPassiveLeafSizeEntry.set_range(1, std::numeric_limits<float>::max());
                    _maxPassiveLeafSizeEntry.set_increments(1.0, 10.0);
                    _maxPassiveLeafSizeEntry.set_value(_solver.passiveTree().maxLeafSize());
                    _maxPassiveLeafSizeEntry.signal_value_changed().connect([&]() {
                        _solver.passiveTree().maxLeafSize() = _maxPassiveLeafSizeEntry.get_value_as_int();
                    });
                } else getWidget<Gtk::Widget>("passive-tree-leaf-size").hide();

            } else getWidget<Gtk::Widget>("passive-tree").hide();
        }

    };

}

#endif //N_BODY_SOLVERSETTINGS_H
