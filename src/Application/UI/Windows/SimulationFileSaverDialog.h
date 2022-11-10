//
// Created by Jackson Campolattaro on 11/4/22.
//

#ifndef N_BODY_SIMULATIONFILESAVERDIALOG_H
#define N_BODY_SIMULATIONFILESAVERDIALOG_H

#include <gtkmm/dialog.h>
#include <gtkmm/filechoosernative.h>

namespace UI {

    class SimulationFileSaverDialog : public Gtk::FileChooserNative {
    public:

        SimulationFileSaverDialog();

        sigc::signal<void(const Glib::RefPtr<Gio::File> &file)> signal_fileSelected;

    protected:

        void on_response(int response_id) override;

    };

}


#endif //N_BODY_SIMULATIONFILESAVERDIALOG_H
