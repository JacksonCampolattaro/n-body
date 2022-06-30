#ifndef N_BODY_INTERACTIVE_H
#define N_BODY_INTERACTIVE_H

#include <gtkmm/applicationwindow.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/View/InteractiveView.h>
#include <NBody/View/GtkmmArcBallCamera.h>

#include "Sidebar/Sidebar.h"

namespace UI {

    class Interactive : public Gtk::ApplicationWindow {
    private:

        NBody::GtkmmArcBallCamera _camera;
        NBody::Simulation &_simulation;

        NBody::InteractiveView _view;
        Sidebar _sidebar;

    public:

        Interactive(Gtk::ApplicationWindow::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder,
                    NBody::Simulation &simulation, NBody::Physics::Rule &rule, NBody::Solver &solver);
    };

}


#endif //N_BODY_INTERACTIVE_H