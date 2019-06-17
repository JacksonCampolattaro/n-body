//
// Created by jackcamp on 5/24/19.
//

#ifndef N_BODY_APPLICATION_H
#define N_BODY_APPLICATION_H


#include <gtkmm.h>
#include "Window.h"


class Application : public Gtk::Application {

public:

    Application();

private:

    void on_activate();

    Window *window;
};


#endif //N_BODY_APPLICATION_H
