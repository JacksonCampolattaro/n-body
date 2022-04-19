#ifndef N_BODY_APPLICATION_H
#define N_BODY_APPLICATION_H

#include <gtkmm.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/NaiveSolver.h>

class Application : public Gtk::Application {
private:

    NBody::Simulation _simulation;
    std::shared_ptr<NBody::Solver> _solver;

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
