//
// Created by Jackson Campolattaro on 7/1/22.
//

#ifndef N_BODY_SAVESIMULATIONDIALOG_H
#define N_BODY_SAVESIMULATIONDIALOG_H

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/filechoosernative.h>
#include <gtkmm/messagedialog.h>

#include <NBody/Simulation/Simulation.h>

namespace UI {

    class SaveSimulationDialog {
    private:

        const NBody::Simulation &_simulation;

    public:

        explicit SaveSimulationDialog(const NBody::Simulation &simulation);

        void show();

    protected:

        void on_response(int response_id, const Glib::RefPtr<Gtk::FileChooserNative>& chooser);
    };

}

#endif //N_BODY_SAVESIMULATIONDIALOG_H
