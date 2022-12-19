//
// Created by Jackson Campolattaro on 11/4/22.
//

#ifndef N_BODY_SIMULATIONFILEMANAGER_H
#define N_BODY_SIMULATIONFILEMANAGER_H

#include <NBody/Simulation/Simulation.h>

#include <giomm/file.h>

namespace NBody {

    class SimulationFileManager {
    private:

        NBody::Simulation &_simulation;
        Glib::RefPtr<Gio::File> _file;

    public:

        SimulationFileManager(NBody::Simulation &simulation);

        void open(const Glib::RefPtr<Gio::File> &file);
        void close();

        void save();
        void saveAs(const Glib::RefPtr<Gio::File> &file);

        bool canSave();



    };

}

#endif //N_BODY_SIMULATIONFILEMANAGER_H
