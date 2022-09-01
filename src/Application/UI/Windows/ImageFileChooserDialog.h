//
// Created by Jackson Campolattaro on 9/1/22.
//

#ifndef N_BODY_IMAGEFILECHOOSERDIALOG_H
#define N_BODY_IMAGEFILECHOOSERDIALOG_H

#include <gtkmm/dialog.h>
#include <gtkmm/filechoosernative.h>
#include <glibmm/refptr.h>
#include <gdkmm/pixbuf.h>

namespace UI {

    class ImageFileChooserDialog : public Gtk::FileChooserNative {
    private:

        Glib::RefPtr<Gdk::Pixbuf> _image;

    public:

        ImageFileChooserDialog();

        void save(Glib::RefPtr<Gdk::Pixbuf> image);

    protected:

        void on_response(int response_id) override;
    };

}

#endif //N_BODY_IMAGEFILECHOOSERDIALOG_H
