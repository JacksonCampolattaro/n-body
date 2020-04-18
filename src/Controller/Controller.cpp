//
// Created by jackcamp on 4/18/20.
//

#include "Controller.h"

void Controller::Controller::loadSimulation(const std::string& filepath) {

    spdlog::debug("Attempting to load a simulation from file path: " + filepath);
}

void Controller::Controller::saveSimulation(const std::string &filepath) {

    spdlog::debug("Attempting to save a simulation at file path: " + filepath);

    std::ofstream out(filepath);

    cereal::XMLOutputArchive archive(out);

    archive(_simulation);
}
