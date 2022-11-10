//
// Created by Jackson Campolattaro on 11/4/22.
//

#include "SimulationFileSaverDialog.h"

#include <spdlog/spdlog.h>

UI::SimulationFileSaverDialog::SimulationFileSaverDialog() :
        Gtk::FileChooserNative(
                "Save the current Scenario",
                Gtk::FileChooser::Action::SAVE,
                "Select", "Cancel"
        ) {

    set_modal();

    signal_response().connect(sigc::mem_fun(*this, &SimulationFileSaverDialog::on_response));

    // todo: should default to the location the data was last loaded from
    set_current_name("out.json");
}

void UI::SimulationFileSaverDialog::on_response(int response_id) {

    // Save the file if the user chose a destination
    if (response_id == Gtk::ResponseType::ACCEPT && get_file()) {
        signal_fileSelected.emit(get_file());
    }
}
