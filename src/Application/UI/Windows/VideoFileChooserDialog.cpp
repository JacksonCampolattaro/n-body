//
// Created by Jackson Campolattaro on 8/31/22.
//

#include "VideoFileChooserDialog.h"

#include <NBody/View/VideoOutputStream.h>

UI::VideoFileChooserDialog::VideoFileChooserDialog() :
        Gtk::FileChooserNative(
                "Choose an output file for the video",
                Gtk::FileChooser::Action::SAVE,
                "Select", "Cancel"
        ) {

    set_modal();

    signal_response().connect(sigc::mem_fun(*this, &VideoFileChooserDialog::on_response));

    for (const auto &[name, extensions]: VideoOutputStream::supportedExtensions()) {

        // Create a filter with a descriptive name, and a list of appropriate extensions
        auto filter = Gtk::FileFilter::create();
        filter->set_name(fmt::format("{} (.{})", name, fmt::join(extensions, ", .")));
        for (const auto &extension: extensions)
            filter->add_suffix(extension);
        add_filter(filter);

        // When we encounter the mp4 filetype, set its filter as the default
        if (name == "mp4") set_filter(filter);
    }

    set_current_name("out.mpeg");
}

void UI::VideoFileChooserDialog::on_response(int response_id) {
    if (response_id == Gtk::ResponseType::ACCEPT && get_file()) {
        signal_fileSelected.emit();
    }
}
