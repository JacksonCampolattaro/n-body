//
// Created by Jackson Campolattaro on 7/1/22.
//

#include <spdlog/spdlog.h>
#include "SaveSimulationDialog.h"

UI::SaveSimulationDialog::SaveSimulationDialog(const NBody::Simulation &simulation) :
        _simulation(simulation) {}

void UI::SaveSimulationDialog::show() {

    auto chooser = Gtk::FileChooserNative::create(
            "Save the current Scenario",
            Gtk::FileChooser::Action::SAVE,
            "Select", "Cancel"
    );

    chooser->set_current_name("out.json");

    chooser->signal_response().connect(sigc::bind(sigc::mem_fun(*this, &SaveSimulationDialog::on_response), chooser));
    chooser->show();
}

void UI::SaveSimulationDialog::on_response(int response_id, const Glib::RefPtr<Gtk::FileChooserNative> &chooser) {

    // Save the file if the user chose a destination
    if (response_id == Gtk::ResponseType::ACCEPT && chooser->get_file()) {
        _simulation.save(*chooser->get_file());
    }
}
