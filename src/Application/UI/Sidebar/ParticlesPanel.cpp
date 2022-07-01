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
        _countLabel(*_builder->get_widget<Gtk::Label>("count-label")),
        _averagePositionView(*Gtk::Builder::get_widget_derived<PositionView>(_builder, "average-position-view")),
        _centerOfMassPositionView(*Gtk::Builder::get_widget_derived<PositionView>(_builder, "center-of-mass-position-view")),
        _simulation(simulation),
        _particlesView(simulation),
        _saveDialog(simulation),
        _loadDialog(simulation) {

    auto root = _builder->get_widget<Gtk::Widget>("root");
    _contents.append(*root);

    _simulation.signal_changed.connect([&](){
        _countLabel.set_text(std::to_string(simulation.size()));
    });

    auto openParticlesViewButton = _builder->get_widget<Gtk::Button>("modify-button");
    openParticlesViewButton->signal_clicked().connect([&] {
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
        spdlog::debug("Save clicked");
        _saveDialog.show();
    });

    auto openButton = _builder->get_widget<Gtk::Button>("open-button");
    openButton->signal_clicked().connect([&] {
        spdlog::debug("Open clicked");
        _loadDialog.show();
    });

}
