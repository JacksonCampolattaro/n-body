//
// Created by jackcamp on 12/21/20.
//

#include "Viewport.h"

NBody::Interface::Viewport::Viewport() {

    _window.add(_view);
    _window.show();
    _view.show();

    slot_setSimulation = sigc::mem_fun(&_view, &View::View::setSimulation);
}

Gtk::Window &NBody::Interface::Viewport::window() {
    return _window;
}
