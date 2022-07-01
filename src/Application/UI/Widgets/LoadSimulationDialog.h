//
// Created by Jackson Campolattaro on 7/1/22.
//

#ifndef N_BODY_LOADSIMULATIONDIALOG_H
#define N_BODY_LOADSIMULATIONDIALOG_H

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/filechoosernative.h>
#include <gtkmm/messagedialog.h>

#include <NBody/Simulation/Simulation.h>

namespace UI {

    class LoadSimulationDialog {
    private:

        NBody::Simulation &_simulation;

    public:

        explicit LoadSimulationDialog(NBody::Simulation &simulation);

        void show();

    protected:

        void on_response(int response_id, const Glib::RefPtr<Gtk::FileChooserNative>& chooser);
    };

}

#endif //N_BODY_LOADSIMULATIONDIALOG_H
