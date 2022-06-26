//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_PARTICLEPASSIVEMASSVIEW_H
#define N_BODY_PARTICLEPASSIVEMASSVIEW_H

#include "NBody/Simulation/Simulation.h"
#include "Application/UI/Widgets/Bindable.h"
#include "Application/UI/Widgets/View/FloatView.h"

#include <gtkmm/label.h>

namespace UI {

    class ParticlePassiveMassView : public FloatView<10>, public Bindable<NBody::Physics::PassiveMass> {
    public:

        void update(NBody::Physics::PassiveMass &value) override { setValue(value.mass()); };
    };

}

#endif //N_BODY_PARTICLEPASSIVEMASSVIEW_H
