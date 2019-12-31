//
// Created by jackcamp on 12/31/19.
//

#ifndef N_BODY_CONFIG_H
#define N_BODY_CONFIG_H

#include "model/tools/BodyList.h"
#include "model/PhysicsContext.h"

#include <string>

#include <CLI/CLI.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>

using std::string;
using std::shared_ptr;
using std::make_shared;


class Config {

public:

    Config(int argc, char **argv);

    unsigned int cycles;
    bool headless, verbose, debug, quiet, silent;
    string logPath, bodyArchivePath, physicsArchivePath;
    shared_ptr<spdlog::logger> logger;
    BodyList bodies;
    PhysicsContext physics;

};


#endif //N_BODY_CONFIG_H
