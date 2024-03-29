//
// Created by jackcamp on 2/16/22.
//

#include <spdlog/spdlog.h>
#include <gtkmm/applicationwindow.h>
#include <fstream>
#include <giomm/simpleactiongroup.h>

#include "ParticlesPanel.h"

UI::ParticlesPanel::ParticlesPanel(Gtk::Box::BaseObjectType *cobject,
                                   const Glib::RefPtr<Gtk::Builder> &builder,
                                   NBody::Simulation &simulation,
                                   NBody::SimulationFileManager &fileManager) :
        BuilderWidget<Gtk::Box>(cobject, builder, "/ui/particles_panel.xml"),
        _simulation(simulation),
        _fileManager(fileManager),
        _countLabel(getWidget<Gtk::Label>("count-label")),
        _averagePositionView(getWidget<PositionView>("average-position-view")),
        _totalMassView(getWidget<PreciseFloatView>("total-mass-label")),
        _centerOfMassPositionView(getWidget<PositionView>("center-of-mass-position-view")),
        _interactionCountLabel(getWidget<Gtk::Label>("interaction-count-label")),
        _particleGridCreatorWindow(simulation) {

    _simulation.signal_changed.connect([&]() {

        _countLabel.set_text(std::to_string(simulation.particleCount()));
        _averagePositionView.setValue(simulation.averagePosition());

        _totalMassView.setValue(simulation.totalMass());
        _centerOfMassPositionView.setValue(simulation.centerOfMass());

        _interactionCountLabel.set_text(std::to_string(simulation.interactionCount()));
    });
    _simulation.signal_changed.emit();

    auto loaderActionGroup = Gio::SimpleActionGroup::create();
    loaderActionGroup->add_action("open", _fileManager.slot_open);
    loaderActionGroup->add_action("import", _fileManager.slot_import);
    loaderActionGroup->add_action("new", _fileManager.slot_close);
    dynamic_cast<Gtk::Window *>(get_root())->insert_action_group("loader", loaderActionGroup);

    auto saverActionGroup = Gio::SimpleActionGroup::create();
    saverActionGroup->add_action("save", _fileManager.slot_save);
    saverActionGroup->add_action("save-as", _fileManager.slot_saveAs);
    dynamic_cast<Gtk::Window *>(get_root())->insert_action_group("saver", saverActionGroup);

    auto particleCreatorActionGroup = Gio::SimpleActionGroup::create();
    particleCreatorActionGroup->add_action("new-single", [&]() {
        auto particle = std::make_shared<NBody::Simulation::Particle>(_simulation.newParticle());
        particle->setMass(1.0f);
        particle->setAcceleration({0.0f, 0.0f, 0.0f});
        particle->setSphere({1.0f});
        particle->setColor(Magnum::Color3::green(0.8));
        simulation.signal_changed.emit();
        activate_action(
                "particles.show-particle",
                Glib::Variant<ENTT_ID_TYPE>::create((ENTT_ID_TYPE) particle->entity())
        );
    });
    particleCreatorActionGroup->add_action("new-grid", _particleGridCreatorWindow.slot_open());
    dynamic_cast<Gtk::Window *>(get_root())->insert_action_group("particle-creator", particleCreatorActionGroup);
}
