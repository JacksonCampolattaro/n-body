//
// Created by jackcamp on 9/1/20.
//

#ifndef N_BODY_INTERACTIVEVIEW_H
#define N_BODY_INTERACTIVEVIEW_H

#include "View.h"

#include "MagnumViewport.h"

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>

namespace View {

    class InteractiveView : public View {
    public:
        InteractiveView();

        void attach_application(Controller::Application *application) override;

    private:

        Gtk::Window _window;
        Gtk::Box _box;
        MagnumViewport _view;
        Gtk::Button _button;
    };

}


#endif //N_BODY_INTERACTIVEVIEW_H
