//
// Created by Jackson Campolattaro on 1/10/23.
//

#ifndef N_BODY_LINEARBVHSOLVERSETTINGS_H
#define N_BODY_LINEARBVHSOLVERSETTINGS_H

#include <NBody/Simulation/Solvers/LinearBVHSolver.h>

#include "Application/UI/Widgets/BuilderWidget.h"
#include "Application/UI/Widgets/Entry/FloatEntry.h"

namespace UI {

    class LinearBVHSolverSettings : public BuilderWidget<Gtk::Box> {
    private:

        NBody::LinearBVHSolver<Gravity> &_solver;

        FloatEntry &_thetaEntry;

    public:

        LinearBVHSolverSettings(NBody::LinearBVHSolver<Gravity> &solver);

    };

    class QuadrupoleLinearBVHSolverSettings : public BuilderWidget<Gtk::Box> {
    private:

        NBody::QuadrupoleLinearBVHSolver<Gravity> &_solver;

        FloatEntry &_thetaEntry;

    public:

        QuadrupoleLinearBVHSolverSettings(NBody::QuadrupoleLinearBVHSolver<Gravity> &solver);

    };

}


#endif //N_BODY_LINEARBVHSOLVERSETTINGS_H
