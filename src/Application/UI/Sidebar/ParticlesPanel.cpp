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
                                   NBody::Simulation &simulation) :
        BuilderWidget<Gtk::Box>(cobject, builder, "/ui/particles_panel.xml"),
        _simulation(simulation),
        _modifyButton(getWidget<Gtk::Button>("modify-button")),
        _countLabel(getWidget<Gtk::Label>("count-label")),
        _averagePositionView(getWidget<PositionView>("average-position-view")),
        _totalMassView(getWidget<PreciseFloatView>("total-mass-label")),
        _centerOfMassPositionView(getWidget<PositionView>("center-of-mass-position-view")),
        _interactionCountLabel(getWidget<Gtk::Label>("interaction-count-label")),
        _particlesListWindow(simulation),
        _particleGridCreatorWindow(simulation),
        _saveDialog(simulation),
        _loadDialog(simulation) {

    _simulation.signal_changed.connect([&]() {

        _countLabel.set_text(std::to_string(simulation.particleCount()));
        _averagePositionView.setValue(simulation.averagePosition());

        _totalMassView.setValue(simulation.totalMass());
        _centerOfMassPositionView.setValue(simulation.centerOfMass());

        _interactionCountLabel.set_text(std::to_string(simulation.interactionCount()));
    });
    _simulation.signal_changed.emit();

    _modifyButton.signal_clicked().connect(_particlesListWindow.slot_open());

    signal_open_particle.connect(_particleEditorWindow.slot_open);
    _particlesListWindow.signal_open_particle().connect(_particleEditorWindow.slot_open);
    _simulation.signal_particle_removed.connect(sigc::hide(_particleEditorWindow.slot_close));

//    _contents.set_expand();
//    append(*_builder->get_widget<Gtk::ActionBar>("action-bar"));

    auto &saveButton = getWidget<Gtk::Button>("save-button");
    saveButton.signal_clicked().connect([&] {
        _saveDialog.show();
    });

    auto &openButton = getWidget<Gtk::Button>("open-button");
    openButton.signal_clicked().connect([&] {
        _loadDialog.show();
    });


    // todo: action group also works if it's attached to the ApplicationWindow,
    // in the future, UI::Interactive should be responsible for most action groups
    auto actionGroup = Gio::SimpleActionGroup::create();
    getWidget<Gtk::Widget>("add-button").insert_action_group("create-particle", actionGroup);

    actionGroup->add_action("single",
                            [&]() {
                                auto particle =
                                        std::make_shared<NBody::Simulation::Particle>(_simulation.newParticle());
                                particle->setMass(1.0f);
                                particle->setSphere({1.0f});
                                particle->setColor(Magnum::Color3::green(0.8));
                                signal_open_particle.emit(particle);
                            });

    actionGroup->add_action("grid",
                            [&]() {
                                _particleGridCreatorWindow.show();
                            });
}
