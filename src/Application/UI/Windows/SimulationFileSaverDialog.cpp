//
// Created by Jackson Campolattaro on 11/4/22.
//

#include "SimulationFileSaverDialog.h"

UI::SimulationFileSaverDialog::SimulationFileSaverDialog(NBody::SimulationFileManager &fileManager) :
        Gtk::FileChooserNative(
                "Save the current Scenario",
                Gtk::FileChooser::Action::SAVE,
                "Select",
                "Cancel"
        ),
        _fileManager(fileManager) {

    // todo: should default to the location the data was last loaded from
    set_current_name("out.json");

    signal_response().connect([&](int response_id) {

        // Save the file if the user chose a destination
        if (response_id == Gtk::ResponseType::ACCEPT && get_file()) {
            _fileManager.saveAs(get_file());
        }
    });
}
