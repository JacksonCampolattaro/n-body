//
// Created by Jackson Campolattaro on 6/23/22.
//

#include "ParticleActiveMassBindable.h"

void UI::ParticleActiveMassBindable::bind(NBody::Simulation::Particle &particle) {
    // todo This should be formatted specially
    set_text(std::to_string(particle.get<NBody::Physics::ActiveMass>().mass()));
}

void UI::ParticleActiveMassBindable::unbind() {
    // todo
}
