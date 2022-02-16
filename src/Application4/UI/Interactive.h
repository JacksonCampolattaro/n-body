#ifndef N_BODY_INTERACTIVE_H
#define N_BODY_INTERACTIVE_H

#include <gtkmm/applicationwindow.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/View/InteractiveView.h>
#include <NBody/View/GtkmmArcBallCamera.h>

namespace UI {

    class Interactive : public Gtk::ApplicationWindow  {
    private:

        NBody::GtkmmArcBallCamera _camera;
        NBody::InteractiveView _view;

        NBody::Simulation &_simulation;

    public:

        Interactive(NBody::Simulation &simulation);
    };

}


#endif //N_BODY_INTERACTIVE_H
