//
// Created by jackcamp on 2/16/22.
//

#ifndef N_BODY_PARTICLESPANEL_H
#define N_BODY_PARTICLESPANEL_H

#include <NBody/Simulation/Simulation.h>

#include <gtkmm/listbox.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/switch.h>

#include "../Widgets/LabeledWidget.h"
#include "../Widgets/ParticlesListView.h"

#include "Panel.h"

namespace UI {

class ParticlesPanel : public Panel {
    private:

        UI::ParticlesListView _particlesView;

    public:

        ParticlesPanel(NBody::Simulation &simulation);

    };

}


#endif //N_BODY_PARTICLESPANEL_H
