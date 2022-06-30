//
// Created by jackcamp on 2/16/22.
//

#include <spdlog/spdlog.h>
#include <gtkmm/applicationwindow.h>
#include <fstream>

#include "ParticlesPanel.h"

UI::ParticlesPanel::ParticlesPanel(NBody::Simulation &simulation) :
        Panel("Particles"),
        _simulation(simulation),
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

    auto filter_json = Gtk::FileFilter::create();
    filter_json->set_name("JSON files");
    filter_json->add_mime_type("application/json");
    _fileChooser.add_filter(filter_json);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    _fileChooser.add_filter(filter_any);

    _fileChooser.add_choice("rules", "Load rules settings from file");

    _fileChooser.signal_response().connect(sigc::mem_fun(*this, &ParticlesPanel::on_file_dialog_response));
    _fileChooser.signal_close_request().connect(
            [&] {
                _fileChooser.hide();
                return true;
            },
            false
    );
}

void UI::ParticlesPanel::on_file_dialog_response(int response_id) {

    // We don't need to do anything unless the user chose a file
    if (response_id == Gtk::ResponseType::OK) {

        spdlog::info("Loading scenario from file: {}", _fileChooser.get_file()->get_path());

        std::ifstream scenario_data{_fileChooser.get_file()->get_path()};
        json data = json::parse(scenario_data);
        spdlog::debug("Loading {} particles", data["particles"].size());
        from_json(data, _simulation);

        // todo Load rules from file if that option is enabled

        spdlog::debug("Simulation now contains {} particles", _simulation.size());
    }

    // Always hide the file chooser once the user makes a choice
    _fileChooser.hide();
}
