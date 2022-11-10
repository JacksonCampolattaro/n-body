//
// Created by Jackson Campolattaro on 11/8/22.
//

#include "SimulationFileLoaderDialog.h"

#include <spdlog/spdlog.h>

UI::SimulationFileLoaderDialog::SimulationFileLoaderDialog() :
        Gtk::FileChooserNative(
                "Load scenario from file",
                Gtk::FileChooser::Action::OPEN,
                "Select",
                "Cancel"
        ) {


    auto filter_json = Gtk::FileFilter::create();
    filter_json->set_name("JSON File (.json)");
    filter_json->add_suffix("json");
    add_filter(filter_json);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any File");
    filter_any->add_pattern("*");
    add_filter(filter_any);

    signal_response().connect(sigc::mem_fun(*this, &SimulationFileLoaderDialog::on_response));
}

void UI::SimulationFileLoaderDialog::on_response(int response_id) {

    // Save the file if the user chose a destination
    if (response_id == Gtk::ResponseType::ACCEPT && get_file()) {
        signal_fileSelected.emit(get_file());
    }
}
