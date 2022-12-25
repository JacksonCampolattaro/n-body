//
// Created by Jackson Campolattaro on 11/4/22.
//

#ifndef N_BODY_SIMULATIONFILEMANAGER_H
#define N_BODY_SIMULATIONFILEMANAGER_H

#include "UI/Windows/SimulationFileLoaderDialog.h"
#include "UI/Windows/SimulationFileSaverDialog.h"

#include <NBody/Simulation/Simulation.h>

#include <giomm/file.h>

namespace NBody {

    class SimulationFileManager {
    private:

        NBody::Simulation &_simulation;
        Glib::RefPtr<Gio::File> _file;

        UI::SimulationFileLoaderDialog _openerDialog;
        UI::SimulationFileLoaderDialog _importerDialog;
        UI::SimulationFileSaverDialog _saverDialog;

    public:

        sigc::slot<void()> slot_open;
        sigc::slot<void()> slot_close;
        sigc::slot<void()> slot_import;
        sigc::slot<void()> slot_saveAs;
        sigc::slot<void()> slot_save;

        SimulationFileManager(NBody::Simulation &simulation);

        void open();
        void openPath(const Glib::RefPtr<Gio::File> &file);

        void close();

        void import();
        void importFromPath(const Glib::RefPtr<Gio::File> &file);

        void saveAs();
        void save();
        void saveToPath(const Glib::RefPtr<Gio::File> &file);

        bool canSave();
    };

}

#endif //N_BODY_SIMULATIONFILEMANAGER_H
