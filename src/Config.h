//
// Created by jackcamp on 12/31/19.
//

#ifndef N_BODY_CONFIG_H
#define N_BODY_CONFIG_H

#include <string>

#include <CLI/CLI.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

using std::string;
using std::shared_ptr;
using std::make_shared;


class Config {

public:

    Config(int argc, char **argv);

    shared_ptr<spdlog::logger> logger();

    unsigned int cycles;
    bool headless, verbose, debug, quiet, silent;
    string logPath, bodyArchivePath, physicsArchivePath;


};


#endif //N_BODY_CONFIG_H
