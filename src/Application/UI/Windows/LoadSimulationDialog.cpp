//
// Created by Jackson Campolattaro on 7/1/22.
//

#include "LoadSimulationDialog.h"

UI::LoadSimulationDialog::LoadSimulationDialog(NBody::Simulation &simulation) : _simulation(simulation) {}

void UI::LoadSimulationDialog::show() {

    auto chooser = Gtk::FileChooserNative::create(
            "Load scenario from file",
            Gtk::FileChooser::Action::OPEN,
            "Select", "Cancel"
    );

    auto filter_json = Gtk::FileFilter::create();
    filter_json->set_name("JSON File");
    filter_json->add_mime_type("application/json");
    filter_json->add_suffix("json");
    chooser->add_filter(filter_json);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any File");
    filter_any->add_pattern("*");
    chooser->add_filter(filter_any);

    chooser->signal_response().connect(sigc::bind(sigc::mem_fun(*this, &LoadSimulationDialog::on_response), chooser));
    chooser->show();
}

void UI::LoadSimulationDialog::on_response(int response_id, const Glib::RefPtr<Gtk::FileChooserNative> &chooser) {

    // Load the file if the user chose a destination which exists
    if (response_id == Gtk::ResponseType::ACCEPT && chooser->get_file() && chooser->get_file()->query_exists()) {
        _simulation.load(*chooser->get_file());
    }
}
