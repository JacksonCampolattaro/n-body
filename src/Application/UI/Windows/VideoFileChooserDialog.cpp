//
// Created by Jackson Campolattaro on 8/31/22.
//

#include "VideoFileChooserDialog.h"

UI::VideoFileChooserDialog::VideoFileChooserDialog() :
        Gtk::FileChooserNative("Choose an output file for the video",
                               Gtk::FileChooser::Action::SAVE,
                               "Select", "Cancel") {

    signal_response().connect(sigc::mem_fun(*this, &VideoFileChooserDialog::on_response));
}

void UI::VideoFileChooserDialog::on_response(int response_id) {
    if (response_id == Gtk::ResponseType::ACCEPT && get_file()) {
        // todo: ensure the file is a valid path for video
        signal_fileSelected.emit();
    }
}
