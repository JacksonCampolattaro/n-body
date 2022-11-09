//
// Created by Jackson Campolattaro on 11/8/22.
//

#ifndef N_BODY_SIMULATIONFILELOADERDIALOG_H
#define N_BODY_SIMULATIONFILELOADERDIALOG_H

#include <Application/SimulationFileManager.h>

#include <NBody/Simulation/Simulation.h>

#include <gtkmm/dialog.h>
#include <gtkmm/filechoosernative.h>

#include <spdlog/spdlog.h>

namespace UI {

    class SimulationFileLoaderDialog : public Gtk::FileChooserNative {
    private:

        NBody::SimulationFileManager &_fileManager;

    public:

        explicit SimulationFileLoaderDialog(NBody::SimulationFileManager &fileManager);

    };

}


#endif //N_BODY_SIMULATIONFILELOADERDIALOG_H
