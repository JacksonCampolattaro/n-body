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
#include "Application/UI/Windows/SaveSimulationDialog.h"
#include "Application/UI/Windows/LoadSimulationDialog.h"
#include "../Widgets/View/VectorView.h"

#include "Panel.h"
#include "Application/UI/Widgets/ParticlesColumnView.h"

namespace UI {

    class ParticlesPanel : public Panel {
    private:

        NBody::Simulation &_simulation;

        Glib::RefPtr<Gtk::Builder> _builder;

        Gtk::Button &_modifyButton;

        Gtk::Label &_countLabel;
        PositionView &_averagePositionView;

        PreciseFloatView &_totalMassView;
        PositionView &_centerOfMassPositionView;

        Gtk::Label &_interactionCountLabel;

        UI::ParticlesColumnView _particlesView;
        Gtk::Window _particlesWindow;

        ParticleEditorWindow _particleEditorWindow;

        SaveSimulationDialog _saveDialog;
        LoadSimulationDialog _loadDialog;

    public:

        ParticlesPanel(NBody::Simulation &simulation);

    public:

        sigc::signal<void(std::shared_ptr<NBody::Simulation::Particle> &)> signal_open_particle;

    };

}


#endif //N_BODY_PARTICLESPANEL_H
