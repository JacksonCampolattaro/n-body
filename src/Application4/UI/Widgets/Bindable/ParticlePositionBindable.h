//
// Created by Jackson Campolattaro on 6/24/22.
//

#ifndef N_BODY_PARTICLEPOSITIONBINDABLE_H
#define N_BODY_PARTICLEPOSITIONBINDABLE_H

#include "Application4/UI/Widgets/View/VectorView.h"
#include "Bindable.h"
#include <NBody/Simulation/Simulation.h>

namespace UI {

    class ParticlePositionBindable : public PositionView, public Bindable<NBody::Physics::Position> {
    public:

        void update(NBody::Physics::Position &value) override { setValue(value); };
    };

}

#endif //N_BODY_PARTICLEPOSITIONBINDABLE_H
