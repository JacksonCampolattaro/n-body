//
// Created by jackcamp on 5/24/19.
//

#ifndef N_BODY_NBODY_APPLICATION_H
#define N_BODY_NBODY_APPLICATION_H


#include <gtkmm.h>
#include "nbody_window.h"


class nbody_application : public Gtk::Application {

public:

    nbody_application();

private:

    void on_activate();

    nbody_window window;
};


#endif //N_BODY_NBODY_APPLICATION_H
