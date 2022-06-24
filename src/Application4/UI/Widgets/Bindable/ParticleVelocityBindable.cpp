//
// Created by Jackson Campolattaro on 6/24/22.
//

#include "ParticleVelocityBindable.h"

void UI::ParticleVelocityBindable::bind(NBody::Simulation::Particle &particle) {
    setValue(particle.get<NBody::Physics::Velocity>());
}

void UI::ParticleVelocityBindable::unbind() {

}
