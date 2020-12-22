//
// Created by jackcamp on 12/21/20.
//

#include "Viewport.h"

Interface::Viewport::Viewport() {

    _window.add(_view);
}

Gtk::Window &Interface::Viewport::window() {
    return _window;
}
