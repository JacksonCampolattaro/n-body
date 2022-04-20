#ifndef N_BODY_SIDEBAR_H
#define N_BODY_SIDEBAR_H

#include <gtkmm/stack.h>
#include <gtkmm/stacksidebar.h>
#include <gtkmm/stackswitcher.h>
#include <gtkmm/notebook.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/View/GtkmmArcBallCamera.h>

#include "ParticlesPanel.h"
#include "PhysicsPanel.h"
#include "CameraPanel.h"
#include "SolverPanel.h"
#include "RunPanel.h"

namespace UI {

    class Sidebar : public Gtk::Box {
    private:

        Gtk::Notebook _notebook;

        Gtk::Image _bodiesIcon;
        ParticlesPanel _bodiesPanel;

        Gtk::Image _physicsIcon;
        PhysicsPanel _physicsPanel;

        Gtk::Image _solverIcon;
        SolverPanel _solverPanel;

        Gtk::Image _cameraIcon;
        CameraPanel _cameraPanel;

        Gtk::Image _runIcon;
        RunPanel _runPanel;

    public:

        Sidebar(NBody::GtkmmArcBallCamera &camera, NBody::Simulation &simulation,
                NBody::Physics::Rule &rule, NBody::Solver &solver);

    };

}


#endif //N_BODY_SIDEBAR_H
