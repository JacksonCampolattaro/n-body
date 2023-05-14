//
// Created by Jackson Campolattaro on 7/21/22.
//

#ifndef N_BODY_BARNESHUTSOLVERSETTINGS_H
#define N_BODY_BARNESHUTSOLVERSETTINGS_H

#include <NBody/Simulation/Solvers/BarnesHutSolver.h>

#include "Application/UI/Widgets/BuilderWidget.h"
#include "Application/UI/Widgets/Entry/FloatEntry.h"

namespace UI {

    class BarnesHutSolverSettings : public BuilderWidget<Gtk::Box> {
    private:

        NBody::BarnesHutSolver<Gravity> &_solver;

        FloatEntry &_thetaEntry;
        Gtk::SpinButton &_maxDepthEntry;
        Gtk::SpinButton &_maxLeafSizeEntry;

    public:

        BarnesHutSolverSettings(NBody::BarnesHutSolver<Gravity> &solver);

    };

    class QuadrupoleBarnesHutSolverSettings : public BuilderWidget<Gtk::Box> {
    private:

        NBody::QuadrupoleBarnesHutSolver<Gravity> &_solver;

        FloatEntry &_thetaEntry;
        Gtk::SpinButton &_maxDepthEntry;
        Gtk::SpinButton &_maxLeafSizeEntry;

    public:

        QuadrupoleBarnesHutSolverSettings(NBody::QuadrupoleBarnesHutSolver<Gravity> &solver);

    };

}

#endif //N_BODY_BARNESHUTSOLVERSETTINGS_H
