//
// Created by Jackson Campolattaro on 6/24/22.
//

#ifndef N_BODY_PARTICLEVELOCITYBINDABLE_H
#define N_BODY_PARTICLEVELOCITYBINDABLE_H

#include "../VectorView.h"
#include <NBody/Simulation/Simulation.h>

namespace UI {

    class ParticleVelocityBindable : public VelocityView {
    public:

        void bind(NBody::Simulation::Particle &particle);

        void unbind();
    };

}

#endif //N_BODY_PARTICLEVELOCITYBINDABLE_H
