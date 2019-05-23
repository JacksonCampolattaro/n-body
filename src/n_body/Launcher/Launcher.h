//
// Created by jackcamp on 5/22/19.
//

#ifndef N_BODY_LAUNCHER_H
#define N_BODY_LAUNCHER_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>

#include "../view/View.h"
#include "../model/Model.h"
#include "ViewportSettingsMenu.h"


class Launcher : public Gtk::Window {

public:

    Launcher();
    virtual ~Launcher();

protected:

    void on_start();

    Gtk::Box mainBox;
    ViewportSettingsMenu viewPortSettings;
    Gtk::Button start_button;


    // Objects used for the simulation
    View *view;
    Model *model;
    PhysicsContext *physicsContext;
    std::vector<Body *> bodies;
};


#endif //N_BODY_LAUNCHER_H
