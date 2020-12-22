//
// Created by jackcamp on 12/21/20.
//

#ifndef N_BODY_VIEWPORT_H
#define N_BODY_VIEWPORT_H

#include "Interface.h"

#include <NBody/View/View.h>

#include <gtkmm/window.h>

namespace NBody::Interface {

    class Viewport : public Interface {
    public:

        Viewport();

        Gtk::Window &window();

    private:

        Gtk::Window _window;
        View::View _view;

    };
}


#endif //N_BODY_VIEWPORT_H
