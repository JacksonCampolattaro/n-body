//
// Created by jackcamp on 2/16/22.
//

#include <spdlog/spdlog.h>
#include <gtkmm/applicationwindow.h>
#include <fstream>

#include "ParticlesPanel.h"

UI::ParticlesPanel::ParticlesPanel(NBody::Simulation &simulation) :
        Panel("Particles"),
        _builder(Gtk::Builder::create_from_resource("/ui/particles_panel.xml")),
        _modifyButton(*_builder->get_widget<Gtk::Button>("modify-button")),
        _countLabel(*_builder->get_widget<Gtk::Label>("count-label")),
        _totalMassView(*Gtk::Builder::get_widget_derived<PreciseFloatView>(
                _builder, "total-mass-label")),
        _averagePositionView(*Gtk::Builder::get_widget_derived<PositionView>(
                _builder, "average-position-view")),
        _centerOfMassPositionView(*Gtk::Builder::get_widget_derived<PositionView>(
                _builder, "center-of-mass-position-view")),
        _interactionCountLabel(*_builder->get_widget<Gtk::Label>("interaction-count-label")),
        _simulation(simulation),
        _particlesView(simulation),
        _saveDialog(simulation),
        _loadDialog(simulation) {

    auto root = _builder->get_widget<Gtk::Widget>("root");
    _contents.append(*root);

    _simulation.signal_changed.connect([&]() {

        _countLabel.set_text(std::to_string(simulation.particleCount()));
        _averagePositionView.setValue(simulation.averagePosition());

        _totalMassView.setValue(simulation.totalMass());
        _centerOfMassPositionView.setValue(simulation.centerOfMass());

        _interactionCountLabel.set_text(std::to_string(simulation.interactionCount()));
    });
    _simulation.signal_changed.emit();

    _modifyButton.signal_clicked().connect([&] {
        _particlesWindow.show();
    });

    _particlesWindow.set_title("Particles");
    _particlesWindow.set_child(_particlesView);
    _particlesWindow.signal_close_request().connect(
            [&] {
                _particlesWindow.hide();
                return true;
            },
            false
    );

    _contents.set_expand();
    append(*_builder->get_widget<Gtk::ActionBar>("action-bar"));

    auto saveButton = _builder->get_widget<Gtk::Button>("save-button");
    saveButton->signal_clicked().connect([&] {
        _saveDialog.show();
    });

    auto openButton = _builder->get_widget<Gtk::Button>("open-button");
    openButton->signal_clicked().connect([&] {
        _loadDialog.show();
    });

}
