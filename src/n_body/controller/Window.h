//
// Created by jackcamp on 5/24/19.
//

#ifndef N_BODY_WINDOW_H
#define N_BODY_WINDOW_H


#include <gtkmm.h>
#include "pages/physics_page/PhysicsContextCreator.h"
#include "pages/view_page/ViewCreator.h"
#include "pages/view_page/RecorderCreator.h"
#include "pages/solver_page/SolverCreator.h"


class Window : public Gtk::ApplicationWindow {

public:

    Window();

    void on_run_clicked();

protected:

    Gtk::Button run_button = Gtk::Button("Run Simulation \u2B9E");
    /// ⮞ ▶ → ⟶

    Gtk::Box mainBox = Gtk::Box(Gtk::ORIENTATION_VERTICAL);

    Gtk::Notebook notebook;

    PhysicsContextCreator physicsPage;
    ViewCreator viewPage;
    RecorderCreator recorderCreator;
    SolverCreator solverCreator;

    Gtk::ActionBar actionBar;

    std::vector<Body> bodies;
};


#endif //N_BODY_WINDOW_H
