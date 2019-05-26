//
// Created by jackcamp on 5/24/19.
//

#include "RecorderCreator.h"

RecorderCreator::RecorderCreator() : Gtk::Frame("Recording") {

    // Configuring the main box

    add(main_box);
    main_box.set_margin_top(20);
    main_box.set_margin_bottom(20);
    main_box.set_margin_left(30);
    main_box.set_margin_right(30);
    main_box.set_spacing(10);


    // Configuring the header

    header_label.set_use_markup(true);
    header_label.set_alignment(Gtk::ALIGN_START);


    // Configuring the description

    description_label.set_use_markup(true);
    description_label.set_alignment(Gtk::ALIGN_START);
    description_label.set_margin_bottom(10);



    // Setting up the enable-recording switch

    enableRecording_switch.property_active().signal_changed().connect(sigc::mem_fun(*this, &RecorderCreator::on_toggleRecording));
    enableRecording_switch.set_halign(Gtk::ALIGN_END);


    // Setting up the select folder run_button

    folderChooser_button.set_filename("/home/jackcamp/CLionProjects/n_body/src/n_body/staging");
    folderChooser_button.set_sensitive(enableRecording_switch.get_active());


    // Configuring child boxes

    enableRecording_box.set_spacing(5);
    enableRecording_box.pack_start(enableRecording_label, Gtk::PACK_SHRINK);
    enableRecording_box.pack_start(enableRecording_switch);

    folderChooser_box.set_spacing(5);
    folderChooser_box.pack_start(folderChooser_label, Gtk::PACK_SHRINK);
    folderChooser_box.pack_start(folderChooser_button);


    // Arranging the main box

    main_box.pack_start(description_label);
    main_box.pack_start(enableRecording_box, Gtk::PACK_SHRINK);
    main_box.pack_start(folderChooser_box, Gtk::PACK_SHRINK);


    // Making sure everything is drawn

    show_all_children();
}

Recorder *RecorderCreator::createRecorder(View *view) {

    if (enableRecording_switch.get_active()) {

        Recorder *recorder = new Recorder(view, folderChooser_button.get_filename() + "/output.mp4");
        return recorder;
    }

    return nullptr;
}

void RecorderCreator::on_toggleRecording() {

    folderChooser_button.set_sensitive(enableRecording_switch.get_active());
}
