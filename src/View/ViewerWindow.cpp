//
// Created by jackcamp on 4/17/20.
//

#include "ViewerWindow.h"

View::ViewerWindow::ViewerWindow() :
    Gtk::Window(), _viewport() {

    spdlog::trace("ViewerWindow constructor invoked");

    add(_viewport);
    _viewport.show();
}

View::ViewerWindow::~ViewerWindow() {}

