#ifndef N_BODY_APPLICATION_H
#define N_BODY_APPLICATION_H


#include <gtkmm.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/MultiSolver.h>
#include <NBody/Physics/Rule.h>

#include "SimulationFileManager.h"

class Application : public Gtk::Application {
private:

    NBody::Simulation _simulation;
    NBody::MultiSolver _solver;
    NBody::Physics::Rule _rule{1.0};
    NBody::SimulationFileManager _fileManager;

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
