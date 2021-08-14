//
// Created by jackcamp on 8/14/21.
//

#include "Pane.h"

Interface::Panes::Pane::Pane() : Gtk::ScrolledWindow() {

    _icon = new Gtk::Image();

    override_background_color(
            Gtk::ListBoxRow().get_style_context()->get_background_color(Gtk::StateFlags::STATE_FLAG_PRELIGHT)
    );
}

Gtk::Image &Interface::Panes::Pane::icon() { return *_icon; }
