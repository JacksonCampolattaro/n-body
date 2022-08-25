//
// Created by Jackson Campolattaro on 8/24/22.
//

#ifndef N_BODY_PARTICLEGRIDCREATORWINDOW_H
#define N_BODY_PARTICLEGRIDCREATORWINDOW_H

#include <gtkmm/window.h>

#include "Application/UI/Widgets/ParticleGridCreator.h"

namespace UI {

    class ParticleGridCreatorWindow : public Gtk::Window {
    private:

        ParticleGridCreator _particleGridCreator;

    public:

        ParticleGridCreatorWindow(NBody::Simulation &simulation);

        sigc::slot<void()> slot_open();
    };

}


#endif //N_BODY_PARTICLEGRIDCREATORWINDOW_H
