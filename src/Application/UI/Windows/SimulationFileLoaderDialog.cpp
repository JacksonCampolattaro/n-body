//
// Created by Jackson Campolattaro on 11/8/22.
//

#include "SimulationFileLoaderDialog.h"

UI::SimulationFileLoaderDialog::SimulationFileLoaderDialog(NBody::SimulationFileManager &fileManager) :
        Gtk::FileChooserNative(
                "Load scenario from file",
                Gtk::FileChooser::Action::OPEN,
                "Select",
                "Cancel"
        ),
        _fileManager(fileManager) {


    // todo: should default to the location the data was last loaded from
    set_current_name("out.json");

    auto filter_json = Gtk::FileFilter::create();
    filter_json->set_name("JSON File (.json)");
    filter_json->add_suffix("json");
    add_filter(filter_json);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any File");
    filter_any->add_pattern("*");
    add_filter(filter_any);

    signal_response().connect([&](int response_id) {

        // Save the file if the user chose a destination
        if (response_id == Gtk::ResponseType::ACCEPT && get_file()) {
            _fileManager.open(get_file());
        }
    });
}
