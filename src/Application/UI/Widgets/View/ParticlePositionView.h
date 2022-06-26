//
// Created by Jackson Campolattaro on 6/24/22.
//

#ifndef N_BODY_PARTICLEPOSITIONVIEW_H
#define N_BODY_PARTICLEPOSITIONVIEW_H

#include "Application/UI/Widgets/View/VectorView.h"
#include "Application/UI/Widgets/Bindable.h"
#include "NBody/Simulation/Simulation.h"

namespace UI {

    class ParticlePositionView : public PositionView, public Bindable<NBody::Physics::Position> {
    public:

        void update(NBody::Physics::Position &value) override { setValue(value); };
    };

}

#endif //N_BODY_PARTICLEPOSITIONVIEW_H
