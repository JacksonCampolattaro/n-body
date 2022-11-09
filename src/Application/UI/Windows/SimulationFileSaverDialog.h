//
// Created by Jackson Campolattaro on 11/4/22.
//

#ifndef N_BODY_SIMULATIONFILESAVERDIALOG_H
#define N_BODY_SIMULATIONFILESAVERDIALOG_H

#include <Application/SimulationFileManager.h>

#include <NBody/Simulation/Simulation.h>

#include <gtkmm/dialog.h>
#include <gtkmm/filechoosernative.h>

#include <spdlog/spdlog.h>

namespace UI {

    class SimulationFileSaverDialog : public Gtk::FileChooserNative {
    private:

        NBody::SimulationFileManager &_fileManager;

    public:

        explicit SimulationFileSaverDialog(NBody::SimulationFileManager &fileManager);

    };

}


#endif //N_BODY_SIMULATIONFILESAVERDIALOG_H
