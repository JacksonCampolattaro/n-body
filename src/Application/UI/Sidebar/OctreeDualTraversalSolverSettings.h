//
// Created by Jackson Campolattaro on 1/26/23.
//

#ifndef N_BODY_OCTREEDUALTRAVERSALSOLVERSETTINGS_H
#define N_BODY_OCTREEDUALTRAVERSALSOLVERSETTINGS_H

#include <NBody/Simulation/Solvers/DualTraversalSolver.h>

#include "Application/UI/Widgets/BuilderWidget.h"
#include "Application/UI/Widgets/Entry/FloatEntry.h"

namespace UI {

    class OctreeDualTraversalSolverSettings : public BuilderWidget<Gtk::Box> {
    private:

        NBody::OctreeDualTraversalSolver &_solver;

        FloatEntry &_thetaEntry;
        Gtk::SpinButton &_maxDepthEntry;
        Gtk::SpinButton &_maxLeafSizeEntry;

    public:

        OctreeDualTraversalSolverSettings(NBody::OctreeDualTraversalSolver &solver);

    };

}


#endif //N_BODY_OCTREEDUALTRAVERSALSOLVERSETTINGS_H
