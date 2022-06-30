//
// Created by jackcamp on 2/16/22.
//

#include <spdlog/spdlog.h>
#include <gtkmm/applicationwindow.h>

#include "ParticlesPanel.h"

UI::ParticlesPanel::ParticlesPanel(NBody::Simulation &simulation) :
        Panel("Particles"),
        _particlesView(simulation),
        _fileChooser("Choose a Scenario") {

    auto builder = Gtk::Builder::create_from_resource("/ui/particles_panel.xml");
    auto root = builder->get_widget<Gtk::Widget>("root");
    _contents.append(*root);

    auto count = builder->get_widget<Gtk::Label>("count-label");
    count->set_text(std::to_string(simulation.size()));

    auto openParticlesViewButton = builder->get_widget<Gtk::Button>("modify-button");
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
    append(*builder->get_widget<Gtk::ActionBar>("action-bar"));

    auto saveButton = builder->get_widget<Gtk::Button>("save-button");
    saveButton->signal_clicked().connect([&] {
        spdlog::debug("save clicked");
    });

    auto openButton = builder->get_widget<Gtk::Button>("open-button");
    openButton->signal_clicked().connect([&] {
        spdlog::debug("open clicked");
        _fileChooser.show();
    });


    // The file chooser should always be displayed over the main window
    // the main window appears last in the list of top level widgets
    auto toplevels = Gtk::Window::list_toplevels();
    _fileChooser.set_transient_for(*toplevels[toplevels.size() - 1]);
    _fileChooser.set_modal(true);

    _fileChooser.add_button("_Cancel", Gtk::ResponseType::CANCEL);
    _fileChooser.add_button("_Open", Gtk::ResponseType::OK);
}
