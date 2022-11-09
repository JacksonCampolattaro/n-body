//
// Created by Jackson Campolattaro on 11/4/22.
//

#include "SimulationFileManager.h"

NBody::SimulationFileManager::SimulationFileManager(NBody::Simulation &simulation) :
        _simulation(simulation) {}

void NBody::SimulationFileManager::open(const Glib::RefPtr<Gio::File> &file) {
    // todo: close currently open file
    _file = file;
    _simulation.load(*_file);
}

void NBody::SimulationFileManager::save() {
    saveAs(_file);
}

void NBody::SimulationFileManager::saveAs(const Glib::RefPtr<Gio::File> &file) {
    _file = file;
    _simulation.save(*_file);
}
