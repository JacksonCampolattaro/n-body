//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_PARTICLEPASSIVEMASSBINDABLE_H
#define N_BODY_PARTICLEPASSIVEMASSBINDABLE_H

#include <NBody/Simulation/Simulation.h>
#include "Bindable.h"

#include <gtkmm/label.h>

namespace UI {

    class ParticlePassiveMassBindable : public Gtk::Label, public Bindable<NBody::Physics::PassiveMass> {
    public:

        void update(NBody::Physics::PassiveMass &value) override { set_text(std::to_string(value.mass())); };
    };

}

#endif //N_BODY_PARTICLEPASSIVEMASSBINDABLE_H
