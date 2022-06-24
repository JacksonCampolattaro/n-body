//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_PARTICLEACTIVEMASSBINDABLE_H
#define N_BODY_PARTICLEACTIVEMASSBINDABLE_H

#include <NBody/Simulation/Simulation.h>
#include "Bindable.h"
#include "Application4/UI/Widgets/View/FloatView.h"

#include <gtkmm/label.h>

namespace UI {

    class ParticleActiveMassBindable : public FloatView<10>, public Bindable<NBody::Physics::ActiveMass> {
    public:

        void update(NBody::Physics::ActiveMass &value) override { setValue(value.mass()); };
    };

}

#endif //N_BODY_PARTICLEACTIVEMASSBINDABLE_H
