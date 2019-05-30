//
// Created by jackcamp on 5/24/19.
//

#ifndef N_BODY_WINDOW_H
#define N_BODY_WINDOW_H


#include <gtkmm.h>
#include "pages/physics_page/PhysicsContextCreator.h"
#include "pages/view_page/ViewCreator.h"
#include "pages/view_page/RecorderCreator.h"


class Window : public Gtk::ApplicationWindow {

public:

    Window();

    void on_run_clicked();

protected:

    Gtk::Button run_button = Gtk::Button("Run Simulation");

    Gtk::Box mainBox = Gtk::Box(Gtk::ORIENTATION_VERTICAL);

    Gtk::Notebook notebook;

    PhysicsContextCreator physicsPage;
    ViewCreator viewPage;
    RecorderCreator recorderCreator;

    Gtk::ActionBar actionBar;

    std::vector<Body *> bodies;
};


#endif //N_BODY_WINDOW_H
