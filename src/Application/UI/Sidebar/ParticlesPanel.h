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

#include "Application/UI/Windows/ParticleEditorWindow.h"
#include "Application/UI/Windows/ParticleGridCreatorWindow.h"
#include "Application/UI/Windows/ParticleListWindow.h"
#include "Application/UI/Windows/SaveSimulationDialog.h"
#include "Application/UI/Windows/LoadSimulationDialog.h"

#include "Application/UI/Widgets/ParticlesColumnView.h"
#include "Application/UI/Widgets/BuilderWidget.h"
#include "Application/UI/Widgets/View/VectorView.h"

namespace UI {

    class ParticlesPanel : public BuilderWidget<Gtk::Box> {
    private:

        NBody::Simulation &_simulation;

        Gtk::Button &_modifyButton;

        Gtk::Label &_countLabel;
        PositionView &_averagePositionView;

        PreciseFloatView &_totalMassView;
        PositionView &_centerOfMassPositionView;

        Gtk::Label &_interactionCountLabel;

        ParticleEditorWindow _particleEditorWindow;
        ParticleGridCreatorWindow _particleGridCreatorWindow;
        ParticleListWindow _particlesListWindow;

        SaveSimulationDialog _saveDialog;
        LoadSimulationDialog _loadDialog;

    public:

        ParticlesPanel(Gtk::Box::BaseObjectType *cobject,
                       const Glib::RefPtr<Gtk::Builder> &builder,
                       NBody::Simulation &simulation);

    public:

        sigc::signal<void(std::shared_ptr<NBody::Simulation::Particle> &)> signal_open_particle;

    };

}


#endif //N_BODY_PARTICLESPANEL_H
