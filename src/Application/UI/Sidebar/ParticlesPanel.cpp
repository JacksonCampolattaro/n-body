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
        _modifyButton(getWidget<Gtk::Button>("modify-button")),
        _countLabel(getWidget<Gtk::Label>("count-label")),
        _averagePositionView(getWidget<PositionView>("average-position-view")),
        _totalMassView(getWidget<PreciseFloatView>("total-mass-label")),
        _centerOfMassPositionView(getWidget<PositionView>("center-of-mass-position-view")),
        _interactionCountLabel(getWidget<Gtk::Label>("interaction-count-label")),
        _particleEditorWindow(),
        _particleGridCreatorWindow(simulation) {

    _simulation.signal_changed.connect([&]() {

        _countLabel.set_text(std::to_string(simulation.particleCount()));
        _averagePositionView.setValue(simulation.averagePosition());

        _totalMassView.setValue(simulation.totalMass());
        _centerOfMassPositionView.setValue(simulation.centerOfMass());

        _interactionCountLabel.set_text(std::to_string(simulation.interactionCount()));
    });
    _simulation.signal_changed.emit();

    _simulationSaverDialog.signal_fileSelected.connect(
            sigc::mem_fun(_fileManager, &NBody::SimulationFileManager::saveAs));
    _simulationLoaderDialog.signal_fileSelected.connect(
            sigc::mem_fun(_fileManager, &NBody::SimulationFileManager::open));

    auto loaderActionGroup = Gio::SimpleActionGroup::create();
    loaderActionGroup->add_action("open", [&](){
        _fileManager.close();
        _simulationLoaderDialog.show();
    });
    loaderActionGroup->add_action("import", sigc::mem_fun(_simulationLoaderDialog, &SimulationFileLoaderDialog::show));
    loaderActionGroup->add_action("new", sigc::mem_fun(_fileManager, &NBody::SimulationFileManager::close));
    dynamic_cast<Gtk::Window *>(get_root())->insert_action_group("loader", loaderActionGroup);

    auto saverActionGroup = Gio::SimpleActionGroup::create();
    saverActionGroup->add_action("save", sigc::mem_fun(_fileManager, &NBody::SimulationFileManager::save));
    saverActionGroup->add_action("save-as", sigc::mem_fun(_simulationSaverDialog, &SimulationFileSaverDialog::show));
    dynamic_cast<Gtk::Window *>(get_root())->insert_action_group("saver", saverActionGroup);

    auto particleCreatorActionGroup = Gio::SimpleActionGroup::create();
    particleCreatorActionGroup->add_action("new-single", [&]() {
        auto particle = std::make_shared<NBody::Simulation::Particle>(_simulation.newParticle());
        particle->setMass(1.0f);
        particle->setSphere({1.0f});
        particle->setColor(Magnum::Color3::green(0.8));
        simulation.signal_changed.emit();
        _particleEditorWindow.slot_open.operator()(particle);
    });
    particleCreatorActionGroup->add_action("new-grid", _particleGridCreatorWindow.slot_open());
    dynamic_cast<Gtk::Window *>(get_root())->insert_action_group("particles", particleCreatorActionGroup);
}

Glib::SignalProxy<void()> UI::ParticlesPanel::signal_openList() {
    return _modifyButton.signal_clicked();
}
