//
// Created by jackcamp on 5/22/19.
//

#include <iostream>
#include "RecorderSettingsMenu.h"

RecorderSettingsMenu::RecorderSettingsMenu() {

    // The main box contains a vertical list of settings to change
    set_orientation(Gtk::ORIENTATION_VERTICAL);

    // Configuring the enable recording box
    enableRecording_box.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    // Adding the label
    enableRecording_label.set_margin_right(20);
    enableRecording_box.pack_start(enableRecording_label, Gtk::PACK_SHRINK);
    // Adding the switch
    enableRecording_switch.property_active().signal_changed().connect(sigc::mem_fun(*this, &RecorderSettingsMenu::on_toggleRecording));
    enableRecording_box.pack_start(enableRecording_switch);
    // Adding the box
    pack_start(enableRecording_box);


    // Configuring the file chooser box
    folderChooser_box.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    // Adding the label
    folderChooser_label.set_margin_right(20);
    folderChooser_box.pack_start(folderChooser_label, Gtk::PACK_SHRINK);
    // Adding the file chooser
    folderChooser_button.set_filename("/home/jackcamp/CLionProjects/n_body/src/n_body/staging");
    folderChooser_button.set_sensitive(enableRecording_switch.get_active());
    folderChooser_box.pack_start(folderChooser_button);
    // Adding the box
    pack_start(folderChooser_box);

    // Making sure everything is drawn
    show_all_children();

}

RecorderSettingsMenu::~RecorderSettingsMenu() {

}

Recorder *RecorderSettingsMenu::getRecorder(View *view) {

    if (enableRecording_switch.get_active()) {

        Recorder *recorder = new Recorder(view, folderChooser_button.get_filename() + "/output.mp4");
        return recorder;
    }

    return nullptr;
}

void RecorderSettingsMenu::on_toggleRecording() {

    folderChooser_button.set_sensitive(enableRecording_switch.get_active());

}
