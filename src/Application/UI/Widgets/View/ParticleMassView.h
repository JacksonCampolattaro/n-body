//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_PARTICLEMASSVIEW_H
#define N_BODY_PARTICLEMASSVIEW_H

#include "NBody/Simulation/Simulation.h"
#include "Application/UI/Widgets/Bindable.h"
#include "Application/UI/Widgets/View/FloatView.h"

#include <gtkmm/label.h>

namespace UI {

    class ParticleMassView : public FloatView<10>, public Bindable<NBody::Physics::Mass> {
    public:

        void update(const NBody::Physics::Mass &value) override {
            show();
            setValue(value.mass());
        };

        void update() override { hide(); };
    };

}

#endif //N_BODY_PARTICLEMASSVIEW_H
