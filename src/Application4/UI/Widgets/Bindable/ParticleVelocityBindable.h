//
// Created by Jackson Campolattaro on 6/24/22.
//

#ifndef N_BODY_PARTICLEVELOCITYBINDABLE_H
#define N_BODY_PARTICLEVELOCITYBINDABLE_H

#include "Application4/UI/Widgets/View/VectorView.h"
#include "Bindable.h"
#include <NBody/Simulation/Simulation.h>

namespace UI {

    class ParticleVelocityBindable : public VelocityView, public Bindable<NBody::Physics::Velocity> {
    public:

        void update(NBody::Physics::Velocity &value) override { setValue(value); };
    };

}

#endif //N_BODY_PARTICLEVELOCITYBINDABLE_H
