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
#include <gtkmm/button.h>
#include <gtkmm/actionbar.h>

#include "../Widgets/LabeledWidget.h"
#include "../Widgets/SaveSimulationDialog.h"
#include "../Widgets/LoadSimulationDialog.h"
#include "../Widgets/View/VectorView.h"

#include "Panel.h"
#include "Application/UI/Widgets/ParticlesColumnView.h"

namespace UI {

    class ParticlesPanel : public Panel {
    private:

        Glib::RefPtr<Gtk::Builder> _builder;

        Gtk::Button &_modifyButton;

        Gtk::Label &_countLabel;
        PositionView &_averagePositionView;

        FloatView<10> &_totalMassView;
        PositionView &_centerOfMassPositionView;

        Gtk::Label &_interactionCountLabel;

        NBody::Simulation &_simulation;

        UI::ParticlesColumnView _particlesView;
        Gtk::Window _particlesWindow;

        SaveSimulationDialog _saveDialog;
        LoadSimulationDialog _loadDialog;

    public:

        ParticlesPanel(NBody::Simulation &simulation);

    };

}


#endif //N_BODY_PARTICLESPANEL_H
