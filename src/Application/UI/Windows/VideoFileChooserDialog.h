//
// Created by Jackson Campolattaro on 8/31/22.
//

#ifndef N_BODY_VIDEOFILECHOOSERDIALOG_H
#define N_BODY_VIDEOFILECHOOSERDIALOG_H

#include <gtkmm/filechoosernative.h>
#include <glibmm/refptr.h>

namespace UI {

    class VideoFileChooserDialog : public Gtk::FileChooserNative {
    public:

        VideoFileChooserDialog();

        std::string prettyPath(std::size_t maxLength) {
            auto path = get_file()->get_basename();
            auto directory = get_file()->get_parent();
            while (path.size() < maxLength && directory) {
                path.insert(0, directory->get_basename() + "/");
                directory = directory->get_parent();
            }
            if (directory->has_parent()) path.insert(0, ".../");
            return path;
        }

        sigc::signal<void()> signal_fileSelected;

    protected:

        void on_response(int response_id) override;
    };

}

#endif //N_BODY_VIDEOFILECHOOSERDIALOG_H
