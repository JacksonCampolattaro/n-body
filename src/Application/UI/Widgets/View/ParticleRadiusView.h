//
// Created by Jackson Campolattaro on 6/26/22.
//

#ifndef N_BODY_PARTICLERADIUSVIEW_H
#define N_BODY_PARTICLERADIUSVIEW_H

#include "NBody/Simulation/Simulation.h"
#include "Application/UI/Widgets/Bindable.h"
#include "Application/UI/Widgets/View/FloatView.h"

#include <gtkmm/label.h>

namespace UI {

    class ParticleRadiusView : public FloatView<5>, public Bindable<NBody::Graphics::Sphere> {
    public:

        void update(const NBody::Graphics::Sphere &value) override {
            show();
            setValue(value.radius());
        };

        void update() override { hide(); };
    };

}

#endif //N_BODY_PARTICLERADIUSVIEW_H
