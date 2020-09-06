//
// Created by jackcamp on 4/17/20.
//

#include <fstream>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include "Simulation.h"


void Model::Simulation::update() {

    spdlog::trace("Simulation update invoked");

    // Kick
    for (auto &active : _activeElements) {
        for (auto &passive : _passiveElements) {
            if (passive.position() != active.position())
                _rule(active, passive);
        }
    }

    // Drift
    for (int i = 0; i < _positions.size(); ++i)
        _positions[i] = _positions[i] + (_velocities[i] * _rule._timeIncrement);

    // Alert the renderer
    signal_update_complete.emit(_drawables);
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

   signal_num_drawables_changed.emit(_drawables);
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
