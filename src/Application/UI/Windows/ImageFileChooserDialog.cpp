//
// Created by Jackson Campolattaro on 9/1/22.
//

#include "ImageFileChooserDialog.h"

#include <utility>
#include <filesystem>
#include <fmt/format.h>

UI::ImageFileChooserDialog::ImageFileChooserDialog() :
        Gtk::FileChooserNative("Choose an file to save the image",
                               Gtk::FileChooser::Action::SAVE,
                               "Select", "Cancel") {

    set_current_name("out.png");
    signal_response().connect(sigc::mem_fun(*this, &ImageFileChooserDialog::on_response));

    for (const auto &format: Gdk::Pixbuf::get_formats()) {

        // Convert the list of extensions to std::strings (useful for formatting)
        const auto &ustringExtensions = format.get_extensions();
        std::vector<std::string> extensions;
        std::transform(ustringExtensions.begin(), ustringExtensions.end(),
                       std::back_inserter(extensions),
                       [](const auto &ustring) { return std::string{ustring}; });

        // Create a filter with a descriptive name, and a list of appropriate extensions
        auto filter = Gtk::FileFilter::create();
        filter->set_name(fmt::format("{} (.{})", format.get_name().raw(), fmt::join(extensions, ", .")));
        for (const auto &extension: format.get_extensions())
            filter->add_suffix(extension);
        add_filter(filter);

        // When we encounter the png filetype, set its filter as the default
        if (format.get_name() == "png") set_filter(filter);
    }
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

