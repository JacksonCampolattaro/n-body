//
// Created by Jackson Campolattaro on 8/24/22.
//

#ifndef N_BODY_PARTICLELISTWINDOW_H
#define N_BODY_PARTICLELISTWINDOW_H

#include <gtkmm/window.h>

#include "Application/UI/Widgets/ListView/ParticlesColumnView.h"

namespace UI {

    class ParticleListWindow : public Gtk::Window {
    private:

        ParticlesColumnView _particlesColumnView;

    public:

        explicit ParticleListWindow(NBody::Simulation &simulation);

    public:

        sigc::signal<void(std::shared_ptr<NBody::Simulation::Particle> &)> &signal_openParticle();

        sigc::slot<void()> slot_open();

    };

}

#endif //N_BODY_PARTICLELISTWINDOW_H
