//
// Created by jackcamp on 5/24/19.
//

#ifndef N_BODY_NBODY_WINDOW_H
#define N_BODY_NBODY_WINDOW_H


#include <gtkmm.h>
#include "../Launcher/PhysicsSettingsMenu.h"
#include "pages/physics_page/physicsContext_creator.h"
#include "pages/view_page/view_creator.h"
#include "pages/view_page/recorder_creator.h"


class nbody_window : public Gtk::ApplicationWindow {

public:

    nbody_window();

    void on_run_clicked();

protected:

    Gtk::Button run_button = Gtk::Button("Run Simulation");

    Gtk::Box mainBox = Gtk::Box(Gtk::ORIENTATION_VERTICAL);

    Gtk::Notebook notebook;

    physicsContext_creator physicsPage;
    view_creator viewPage;
    recorder_creator recorderCreator;

    Gtk::ActionBar actionBar;
};


#endif //N_BODY_NBODY_WINDOW_H
