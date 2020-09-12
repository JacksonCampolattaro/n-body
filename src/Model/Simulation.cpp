//
// Created by jackcamp on 4/17/20.
//

#include <fstream>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <iostream>

#include <omp.h>

#include "Simulation.h"


void Model::Simulation::update() {

    spdlog::trace("Simulation update invoked");
    signal_update_progress.emit(0.0f, "kick");

    // Kick
    #pragma omp parallel for default(none) shared(_passiveElements, _activeElements)
    for (size_t p = 0; p < _passiveElements.size(); ++p) {
        auto &passive = _passiveElements[p];

        for (size_t a = 0; a < _activeElements.size(); ++a) {
            auto &active = _activeElements[a];

            _rule(active, passive);
        }
    }

    signal_update_progress.emit(0.5f, "drift");

    // Drift
    #pragma omp simd
    for (size_t i = 0; i < _positions.size(); ++i)
        _positions[i] = _positions[i] + (_velocities[i] * _rule._timeIncrement);

    // Alert the renderer
    signal_drawables_changed.emit(_drawables);
    signal_update_progress.emit(1.0f, "render");
}

Model::Entity &Model::Simulation::newEntity() {
    _entities.push_back(Entity(*this));
    return _entities.back();
}

bool Model::Simulation::loadBodiesFromPath(const std::string &path) {

    spdlog::trace("Loading file");

    std::ifstream file(path);
    if (!file)
        spdlog::error("Failed to load file at path: {}", path);

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    rapidjson::Document document;
    document.Parse(stream.str().c_str());
    if (document.HasParseError()) {
        spdlog::error("Failed to parse file");
        return false;
    }

    document >> *this;

    signal_drawables_changed.emit(_drawables);
    return true;
}

bool Model::Simulation::saveBodiesToPath(const std::string &path) {

    spdlog::trace("Loading file");

    std::ofstream file(path);
    if (!file)
        spdlog::error("Failed to create file at path: {}", path);

    rapidjson::StringBuffer stream;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stream);
    writer << *this;
    file << stream.GetString();

    file.flush();
    file.close();
    return true;
}
