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

        NBody::LinearBVHSolver &_solver;

        FloatEntry &_thetaEntry;

    public:

        LinearBVHSolverSettings(NBody::LinearBVHSolver &solver);

    };

    class QuadrupoleLinearBVHSolverSettings : public BuilderWidget<Gtk::Box> {
    private:

        NBody::QuadrupoleLinearBVHSolver &_solver;

        FloatEntry &_thetaEntry;

    public:

        QuadrupoleLinearBVHSolverSettings(NBody::QuadrupoleLinearBVHSolver &solver);

    };

}


#endif //N_BODY_LINEARBVHSOLVERSETTINGS_H
