//
// Created by jackcamp on 2/16/22.
//

#include "Interactive.h"

#include <gtkmm/binlayout.h>
#include <gtkmm/notebook.h>

UI::Interactive::Interactive(Gtk::ApplicationWindow::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder,
                             NBody::Simulation &simulation, NBody::Physics::Rule &rule,
                             NBody::MultiSolver &solver, NBody::SimulationFileManager &fileManager) :
        BuilderWidget<Gtk::ApplicationWindow>(cobject, builder),
        _simulation{simulation},
        _camera{
                Vector3::zAxis(-20.0f),
                Vector3::yAxis(),
                45.0_degf,
        },
        _view(getWidget<NBody::InteractiveView>("view", _camera, _simulation)),
        _recorder{_camera, _simulation, solver.signal_finished()},
        _particlesPanel(getWidget<ParticlesPanel>("particles-panel", _simulation, fileManager)),
        _physicsPanel(getWidget<PhysicsPanel>("physics-panel", rule)),
        _solverPanel(getWidget<SolverPanel>("solver-panel", solver)),
        _cameraPanel(getWidget<CameraPanel>("camera-panel", _camera, _recorder)),
        _runPanel(getWidget<RunPanel>("run-panel", solver)),
        _particlesListWindow(simulation),
        _particleEditorWindow() {

    // Make sure the sidebar doesn't get squeezed too tight
    Gtk::Requisition min, natural;
    getWidget<Gtk::Notebook>("panel").get_preferred_size(min, natural);
    getWidget<Gtk::Notebook>("panel").set_size_request((natural.get_width() + min.get_width()) / 2,
                                                       natural.get_height());

    _particlesListWindow.signal_openParticle().connect(_particleEditorWindow.slot_open);
    _simulation.signal_particles_removed.connect([&](const auto &particles) {
        if (_particleEditorWindow.currentParticle() &&
            std::find(particles.begin(), particles.end(), *_particleEditorWindow.currentParticle()) != particles.end())
            _particleEditorWindow.hide();
    });

    auto particlesActionGroup = Gio::SimpleActionGroup::create();
    particlesActionGroup->add_action("show", _particlesListWindow.slot_open());
    dynamic_cast<Gtk::Window *>(get_root())->insert_action_group("particles", particlesActionGroup);

    particlesActionGroup->add_action_with_parameter(
            "show-particle", Glib::VARIANT_TYPE_UINT32,
            [&](const Glib::VariantBase &v) {
                assert(v.is_of_type(Glib::VARIANT_TYPE_UINT32));
                auto id = Glib::VariantBase::cast_dynamic<Glib::Variant<ENTT_ID_TYPE>>(v).get();
                auto entity = std::make_shared<NBody::Simulation::Particle>(
                        _simulation, static_cast<NBody::Entity>(id));
                _particleEditorWindow.slot_open.operator()(entity);
            }
    );

}
