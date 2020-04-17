//
// Created by jackcamp on 4/17/20.
//

#include "ViewerWindow.h"

View::ViewerWindow::ViewerWindow() :
    Gtk::Window(), _viewport() {
    _viewport.show();
}

View::ViewerWindow::~ViewerWindow() {}

