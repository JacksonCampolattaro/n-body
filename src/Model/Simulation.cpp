//
// Created by jackcamp on 4/17/20.
//

#include <fstream>
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

    std::ifstream file(path);
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

   rapidjson::Document document;
   document.Parse(path.c_str());
   if (document.HasParseError())
       return false;

   document >> *this;

   return true;
}
