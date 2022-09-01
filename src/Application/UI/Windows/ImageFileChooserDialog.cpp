//
// Created by Jackson Campolattaro on 9/1/22.
//

#include "ImageFileChooserDialog.h"

#include <utility>
#include <filesystem>

UI::ImageFileChooserDialog::ImageFileChooserDialog() :
        Gtk::FileChooserNative("Choose an file to save the image",
                               Gtk::FileChooser::Action::SAVE,
                               "Select", "Cancel") {

    set_current_name("out.png");
    signal_response().connect(sigc::mem_fun(*this, &ImageFileChooserDialog::on_response));
}

void UI::ImageFileChooserDialog::save(Glib::RefPtr<Gdk::Pixbuf> image) {
    _image = std::move(image);
    show();
}

void UI::ImageFileChooserDialog::on_response(int response_id) {
    if (response_id == Gtk::ResponseType::ACCEPT && get_file()) {
        auto path = std::filesystem::path(get_file()->get_path());
        auto extension = path.extension().string().substr(1, -1);
        _image->save(path, extension);
    }
}

