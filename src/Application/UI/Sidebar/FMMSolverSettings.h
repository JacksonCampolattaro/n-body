//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_FMMSOLVERSETTINGS_H
#define N_BODY_FMMSOLVERSETTINGS_H

#include <NBody/Simulation/Solvers/FMMSolver.h>

#include "Application/UI/Widgets/BuilderWidget.h"
#include "Application/UI/Widgets/Entry/FloatEntry.h"

namespace UI {

    class FMMSolverSettings : public BuilderWidget<Gtk::Box> {
    private:

        NBody::FMMSolver<Gravity> &_solver;

        FloatEntry &_thetaEntry;
        Gtk::SpinButton &_maxDepthEntry;
        Gtk::SpinButton &_maxLeafSizeEntry;

    public:

        FMMSolverSettings(NBody::FMMSolver<Gravity> &solver);

    };

}



#endif //N_BODY_FMMSOLVERSETTINGS_H
