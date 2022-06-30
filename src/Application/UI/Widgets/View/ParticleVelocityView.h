//
// Created by Jackson Campolattaro on 6/24/22.
//

#ifndef N_BODY_PARTICLEVELOCITYVIEW_H
#define N_BODY_PARTICLEVELOCITYVIEW_H

#include "Application/UI/Widgets/View/VectorView.h"
#include "Application/UI/Widgets/Bindable.h"
#include "NBody/Simulation/Simulation.h"

namespace UI {

    class ParticleVelocityView : public VelocityView, public Bindable<NBody::Physics::Velocity> {
    public:

        void update(NBody::Physics::Velocity &value) override {
            show();
            setValue(value);
        };

        void update() override { hide(); };
    };

}

#endif //N_BODY_PARTICLEVELOCITYVIEW_H
