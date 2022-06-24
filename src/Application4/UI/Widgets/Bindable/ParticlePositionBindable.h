//
// Created by Jackson Campolattaro on 6/24/22.
//

#ifndef N_BODY_PARTICLEPOSITIONBINDABLE_H
#define N_BODY_PARTICLEPOSITIONBINDABLE_H

#include "../VectorView.h"
#include <NBody/Simulation/Simulation.h>

namespace UI {

    class ParticlePositionBindable : public PositionView {
    public:

        void bind(NBody::Simulation::Particle &particle);

        void unbind();
    };

}

#endif //N_BODY_PARTICLEPOSITIONBINDABLE_H
