//
// Created by Jackson Campolattaro on 6/24/22.
//

#include "ParticlePositionBindable.h"

void UI::ParticlePositionBindable::bind(NBody::Simulation::Particle &particle) {
    setValue(particle.get<NBody::Physics::Position>());
}

void UI::ParticlePositionBindable::unbind() {

}
