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
#include <gtkmm/window.h>

#include "../Widgets/LabeledWidget.h"

#include "Panel.h"
#include "Application/UI/Widgets/ParticlesColumnView.h"

namespace UI {

class ParticlesPanel : public Panel {
    private:

        UI::ParticlesColumnView _particlesView;

        Gtk::Window _particlesWindow;

    public:

        ParticlesPanel(NBody::Simulation &simulation);

    };

}


#endif //N_BODY_PARTICLESPANEL_H
