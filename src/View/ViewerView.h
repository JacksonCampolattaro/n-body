//
// Created by jackcamp on 9/1/20.
//

#ifndef N_BODY_VIEWERVIEW_H
#define N_BODY_VIEWERVIEW_H

#include "View.h"

#include "MagnumViewport.h"

#include <gtkmm/window.h>

namespace Controller {
    class Application;
}

namespace View {

    class ViewerView : public View {
    public:
        explicit ViewerView(Controller::Application &application);

        Gtk::Window _window;
        MagnumViewport _view;

    };

}


#endif //N_BODY_VIEWERVIEW_H
