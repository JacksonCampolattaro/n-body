//
// Created by Jackson Campolattaro on 6/23/22.
//

#include "ParticlePassiveMassBindable.h"

void UI::ParticlePassiveMassBindable::bind(NBody::Simulation::Particle &particle) {
    set_text(std::to_string(particle.get<NBody::Physics::PassiveMass>().mass()));
}

void UI::ParticlePassiveMassBindable::unbind() {
    // todo
}
