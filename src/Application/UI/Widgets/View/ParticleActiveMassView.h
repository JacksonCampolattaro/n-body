//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_PARTICLEACTIVEMASSVIEW_H
#define N_BODY_PARTICLEACTIVEMASSVIEW_H

#include "NBody/Simulation/Simulation.h"
#include "Application/UI/Widgets/Bindable.h"
#include "Application/UI/Widgets/View/FloatView.h"

#include <gtkmm/label.h>

namespace UI {

    class ParticleActiveMassView : public FloatView<10>, public Bindable<NBody::Physics::ActiveMass> {
    public:

        void update(NBody::Physics::ActiveMass &value) override {
            show();
            setValue(value.mass());
        };

        void update() override { hide(); };
    };

}

#endif //N_BODY_PARTICLEACTIVEMASSVIEW_H
