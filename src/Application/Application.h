#ifndef N_BODY_APPLICATION_H
#define N_BODY_APPLICATION_H

#include "Application/UI/SimulationFileManager.h"

#include <gtkmm.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/MultiSolver.h>
#include <NBody/Simulation/Runners/MultiRunner.h>
#include "NBody/Physics/Rules/Gravity.h"

class Application : public Gtk::Application {
private:

    NBody::Simulation _simulation;
    NBody::MultiSolver<Gravity> _solver;
    NBody::Physics::Gravity _rule{1.0};
    NBody::SimulationFileManager _fileManager;
    NBody::MultiRunner _runner;

protected:

    Application();

public:

    static Glib::RefPtr<Application> create();

protected:

    void on_activate() override;

    int on_handle_local_options(const Glib::RefPtr<Glib::VariantDict> &options) override;

    void on_open(const Gio::Application::type_vec_files &files,
                 const Glib::ustring &hint) override;

private:

    void registerActions();

};


#endif //N_BODY_APPLICATION_H
