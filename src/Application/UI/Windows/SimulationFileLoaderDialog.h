//
// Created by Jackson Campolattaro on 11/8/22.
//

#ifndef N_BODY_SIMULATIONFILELOADERDIALOG_H
#define N_BODY_SIMULATIONFILELOADERDIALOG_H

#include <gtkmm/dialog.h>
#include <gtkmm/filechoosernative.h>

namespace UI {

    class SimulationFileLoaderDialog : public Gtk::FileChooserNative {
    public:

        SimulationFileLoaderDialog();

        sigc::signal<void(const Glib::RefPtr<Gio::File> &file)> signal_fileSelected;

    protected:

        void on_response(int response_id) override;

    };

}


#endif //N_BODY_SIMULATIONFILELOADERDIALOG_H
