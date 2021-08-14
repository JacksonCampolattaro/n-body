//
// Created by jackcamp on 8/14/21.
//

#ifndef N_BODY_PANE_H
#define N_BODY_PANE_H

#include <gtkmm/listbox.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/image.h>

namespace Interface::Panes {

    class Pane : public Gtk::ScrolledWindow {
    protected:

        Gtk::Image *_icon;

        Pane();

    public:

        Gtk::Image &icon();

    };

}


#endif //N_BODY_PANE_H
