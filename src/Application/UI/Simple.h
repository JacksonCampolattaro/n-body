#ifndef N_BODY_SIMPLE_H
#define N_BODY_SIMPLE_H

#include <gtkmm/applicationwindow.h>

#include <NBody/Simulation/Simulation.h>
#include "NBody/View/SimpleView.h"
#include <NBody/View/ArcBallControllableCamera.h>

namespace UI {

    class Simple : public Gtk::ApplicationWindow  {
    private:

        NBody::ArcBallControllableCamera _camera;
        NBody::SimpleView _view;

        NBody::Simulation &_simulation;

    public:

        Simple(NBody::Simulation &simulation);
    };

}

#endif //N_BODY_SIMPLE_H
