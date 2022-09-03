#ifndef N_BODY_INTERACTIVE_H
#define N_BODY_INTERACTIVE_H

#include <gtkmm/applicationwindow.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/View/InteractiveView.h>
#include <NBody/View/ArcBallControllableCamera.h>
#include <NBody/View/Recorder.h>

#include "Application/UI/Widgets/BuilderWidget.h"

#include "Sidebar/ParticlesPanel.h"
#include "Sidebar/PhysicsPanel.h"
#include "Sidebar/CameraPanel.h"
#include "Sidebar/SolverPanel.h"
#include "Sidebar/RunPanel.h"

namespace UI {

    class Interactive : public BuilderWidget<Gtk::ApplicationWindow> {
    private:

        NBody::Simulation &_simulation;

        NBody::ArcBallControllableCamera _camera;
        NBody::InteractiveView _view;
        NBody::Recorder _recorder;

        ParticlesPanel &_particlesPanel;
        PhysicsPanel &_physicsPanel;
        SolverPanel &_solverPanel;
        CameraPanel &_cameraPanel;
        RunPanel &_runPanel;

        ParticleEditorWindow _particleEditorWindow;
        ParticleGridCreatorWindow _particleGridCreatorWindow;
        ParticleListWindow _particlesListWindow;

        LoadSimulationDialog _loadSimulationDialog;
        SaveSimulationDialog _saveSimulationDialog;

    public:

        Interactive(Gtk::ApplicationWindow::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder,
                    NBody::Simulation &simulation, NBody::Physics::Rule &rule, NBody::MultiSolver &solver);
    };

}


#endif //N_BODY_INTERACTIVE_H
