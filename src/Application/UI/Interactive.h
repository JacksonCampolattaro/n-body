#ifndef N_BODY_INTERACTIVE_H
#define N_BODY_INTERACTIVE_H

#include <gtkmm/applicationwindow.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Runner.h>
#include <NBody/View/InteractiveView.h>
#include <NBody/View/ArcBallControllableCamera.h>
#include <NBody/View/Recorder.h>

#include "Application/UI/Widgets/BuilderWidget.h"

#include "Sidebar/ParticlesPanel.h"
#include "Sidebar/PhysicsPanel.h"
#include "Sidebar/CameraPanel.h"
#include "Sidebar/SolverPanel.h"
#include "Sidebar/RunPanel.h"
#include "Application/UI/Windows/SimulationFileSaverDialog.h"
#include "Application/UI/Windows/SimulationFileLoaderDialog.h"

namespace UI {

    class Interactive : public BuilderWidget<Gtk::ApplicationWindow> {
    private:

        NBody::Simulation &_simulation;

        NBody::ArcBallControllableCamera _camera;

        NBody::MultiRenderer _simulationRenderer;
        NBody::SolverRenderer<Gravity> _solverRenderer;
        NBody::FieldRenderer<Gravity> _fieldRenderer;

        NBody::Recorder _recorder;

        NBody::InteractiveView &_view;

        ParticlesPanel &_particlesPanel;
        PhysicsPanel &_physicsPanel;
        SolverPanel &_solverPanel;
        CameraPanel &_cameraPanel;
        RunPanel &_runPanel;

        ParticleEditorWindow _particleEditorWindow;
        ParticleListWindow _particlesListWindow;


    public:

        Interactive(Gtk::ApplicationWindow::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder,
                    NBody::Simulation &simulation, NBody::MultiRunner &runner, NBody::Physics::Gravity &rule,
                    NBody::MultiSolver<Gravity> &solver, NBody::SimulationFileManager &fileManager);
    };

}


#endif //N_BODY_INTERACTIVE_H
