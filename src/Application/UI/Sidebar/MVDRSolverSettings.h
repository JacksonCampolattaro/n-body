//
// Created by Jackson Campolattaro on 1/17/23.
//

#ifndef N_BODY_MVDRSOLVERSETTINGS_H
#define N_BODY_MVDRSOLVERSETTINGS_H

#include <NBody/Simulation/Solvers/DualTreeSolver.h>

#include "Application/UI/Widgets/BuilderWidget.h"
#include "Application/UI/Widgets/Entry/FloatEntry.h"

namespace UI {

    class MVDRSolverSettings : public BuilderWidget<Gtk::Box> {
    private:

        NBody::MVDRSolver &_solver;

        FloatEntry &_thetaEntry;
        Gtk::SpinButton &_maxPassiveTreeDepthEntry;
        Gtk::SpinButton &_maxPassiveTreeLeafSizeEntry;

    public:

        explicit MVDRSolverSettings(NBody::MVDRSolver &solver);

    };

}

class MVDRSolverSettings {

};


#endif //N_BODY_MVDRSOLVERSETTINGS_H
