#ifndef N_BODY_APPLICATION_H
#define N_BODY_APPLICATION_H

#include "BackgroundSolver.h"

#include <gtkmm.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/NaiveSolver.h>

#include <NBody/Physics/Gravity.h>

class Application : public Gtk::Application {
private:

    NBody::Simulation _simulation;
    BackgroundSolver<NBody::NaiveSolver> _solver;
    NBody::Physics::Gravity _rule{1.0};

protected:

    Application();

public:

    static Glib::RefPtr<Application> create();

protected:

    void on_activate() override;

    void on_open(const Gio::Application::type_vec_files &files,
                 const Glib::ustring &hint) override;

private:

};


#endif //N_BODY_APPLICATION_H
