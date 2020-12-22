//
// Created by jackcamp on 12/21/20.
//

#include "Viewport.h"

Interface::Viewport::Viewport() {

    _window.add(_view);
    _window.show();
    _view.show();

    slot_setDrawables = sigc::mem_fun(&_view, &View::View::setDrawables);
}

Gtk::Window &Interface::Viewport::window() {
    return _window;
}
