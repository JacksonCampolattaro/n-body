//
// Created by jackcamp on 2/16/22.
//

#include "Interactive.h"

#include <gtkmm/binlayout.h>
#include <gtkmm/notebook.h>

UI::Interactive::Interactive(Gtk::ApplicationWindow::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder,
                             NBody::Simulation &simulation, NBody::Physics::Rule &rule, NBody::MultiSolver &solver) :
        BuilderWidget<Gtk::ApplicationWindow>(cobject, builder),
        _simulation{simulation},
        _camera{
                Vector3::zAxis(-20.0f),
                Vector3::yAxis(),
                45.0_degf,
        },
        _view{_camera, _simulation},
        _recorder{_camera, _simulation, solver.signal_finished()},
        _particlesPanel(getWidget<ParticlesPanel>("particles-panel", _simulation)),
        _physicsPanel(getWidget<PhysicsPanel>("physics-panel", rule)),
        _solverPanel(getWidget<SolverPanel>("solver-panel", solver)),
        _cameraPanel(getWidget<CameraPanel>("camera-panel", _camera, _recorder)),
        _runPanel(getWidget<RunPanel>("run-panel", solver)),
        _particlesListWindow(simulation),
        _particleEditorWindow(),
        _particleGridCreatorWindow(simulation),
        _loadSimulationDialog(simulation),
        _saveSimulationDialog(simulation) {

    auto &content = getWidget<Gtk::Box>("ContentBox");

    _view.set_hexpand();
    content.append(_view);

    // Make sure the sidebar doesn't get squeezed too tight
    //    Gtk::Requisition min, natural;
    //    getWidget<Gtk::Notebook>("panel").get_preferred_size(min, natural);
    //    getWidget<Gtk::Notebook>("panel").set_size_request(natural.get_width(), natural.get_height());

    _particlesPanel.signal_openList().connect(_particlesListWindow.slot_open());

    _particlesListWindow.signal_openParticle().connect(_particleEditorWindow.slot_open);
    _simulation.signal_particle_removed.connect([&](const auto &particle) {
        if (_particleEditorWindow.currentParticle() && *_particleEditorWindow.currentParticle() == particle)
            _particleEditorWindow.hide();
    });

    auto particleCreatorActionGroup = Gio::SimpleActionGroup::create();
    insert_action_group("particles", particleCreatorActionGroup);

    particleCreatorActionGroup->add_action("new-single", [&]() {
        auto particle = std::make_shared<NBody::Simulation::Particle>(_simulation.newParticle());
        particle->setMass(1.0f);
        particle->setSphere({1.0f});
        particle->setColor(Magnum::Color3::green(0.8));
        _particleEditorWindow.slot_open.operator()(particle);
    });

    particleCreatorActionGroup->add_action("new-grid", _particleGridCreatorWindow.slot_open());

    particleCreatorActionGroup->add_action("load-file", [&]() { _loadSimulationDialog.show(); });
    particleCreatorActionGroup->add_action("save-file", [&]() { _saveSimulationDialog.show(); });
}
