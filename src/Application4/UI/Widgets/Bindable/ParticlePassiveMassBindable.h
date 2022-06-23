//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_PARTICLEPASSIVEMASSBINDABLE_H
#define N_BODY_PARTICLEPASSIVEMASSBINDABLE_H

#include <NBody/Simulation/Simulation.h>

#include <gtkmm/label.h>

namespace UI {

    class ParticlePassiveMassBindable : public Gtk::Label {
    public:

        void bind(NBody::Simulation::Particle &particle);

        void unbind();
    };

}

#endif //N_BODY_PARTICLEPASSIVEMASSBINDABLE_H
