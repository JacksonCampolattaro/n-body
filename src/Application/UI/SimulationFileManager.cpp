//
// Created by Jackson Campolattaro on 11/4/22.
//

#include "SimulationFileManager.h"

#include <filesystem>

#include <spdlog/spdlog.h>

NBody::SimulationFileManager::SimulationFileManager(NBody::Simulation &simulation) :
        _simulation(simulation) {


    slot_open = sigc::mem_fun(*this, &SimulationFileManager::open);
    slot_close = sigc::mem_fun(*this, &SimulationFileManager::close);
    slot_import = sigc::mem_fun(*this, &SimulationFileManager::import);
    slot_saveAs = sigc::mem_fun(*this, &SimulationFileManager::saveAs);
    slot_save = sigc::mem_fun(*this, &SimulationFileManager::save);

    _importerDialog.signal_fileSelected.connect(
            sigc::mem_fun(*this, &NBody::SimulationFileManager::importFromPath));
    _openerDialog.signal_fileSelected.connect(
            sigc::mem_fun(*this, &NBody::SimulationFileManager::openPath));
    _saverDialog.signal_fileSelected.connect(
            sigc::mem_fun(*this, &NBody::SimulationFileManager::saveToPath));
}

void NBody::SimulationFileManager::import() {
    _importerDialog.show();
}

void NBody::SimulationFileManager::importFromPath(const Glib::RefPtr<Gio::File> &file) {

    auto path = std::filesystem::path(file->get_path());

    if (path.extension().string() == ".json") {
        spdlog::debug("Opening JSON file at path \"{}\"", file->get_path());
        std::ifstream inputFile(file->get_path());
        json j;
        inputFile >> j;

        from_json(j, _simulation);
        return;
    }

    if (path.extension().string() == ".bin") {
        spdlog::debug("Opening TIPSY file at path \"{}\"", file->get_path());
        std::ifstream inputFile(file->get_path(), std::ios::binary);
        from_tipsy(inputFile, _simulation);
        return;
    }

    spdlog::error("Unrecognized file type");
}

void NBody::SimulationFileManager::open() {
    _openerDialog.show();
    // todo: close existing file
}

void NBody::SimulationFileManager::openPath(const Glib::RefPtr<Gio::File> &file) {
    close();
    _file = file;
    importFromPath(file);
}

void NBody::SimulationFileManager::close() {

    // Notify the UI that _all_ entities have been removed
    _simulation.signal_particles_removed(_simulation.validEntities());

    // Then remove all valid entities
    _simulation.clear<
            Physics::Position,
            Physics::Velocity,
            Physics::Mass,
            Physics::Acceleration,
            NBody::Graphics::Color,
            NBody::Graphics::Sphere,
            sigc::signal<void()>
    >();

    _file.reset();
    _simulation.signal_changed.emit();
}

void NBody::SimulationFileManager::saveAs() {
    _saverDialog.show();
}

void NBody::SimulationFileManager::save() {
    saveToPath(_file);
}

void NBody::SimulationFileManager::saveToPath(const Glib::RefPtr<Gio::File> &file) {
    _file = file;
    _simulation.save(*_file);
}

bool NBody::SimulationFileManager::canSave() {
    return (bool) _file;
}
