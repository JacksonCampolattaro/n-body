//
// Created by Jackson Campolattaro on 8/24/22.
//

#ifndef N_BODY_PARTICLEEDITORWINDOW_H
#define N_BODY_PARTICLEEDITORWINDOW_H

#include <gtkmm/window.h>

#include "../Widgets/Entry/ParticleEntry.h"

namespace UI {

    class ParticleEditorWindow : public Gtk::Window {
    private:

        ParticleEntry _particleEntry;

    public:

        ParticleEditorWindow();

    public:

        sigc::slot<void(std::shared_ptr<NBody::Simulation::Particle> &)> slot_open;

        sigc::slot<void()> slot_close;

    };

}

#endif //N_BODY_PARTICLEEDITORWINDOW_H
