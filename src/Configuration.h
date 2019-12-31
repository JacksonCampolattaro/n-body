//
// Created by jackcamp on 12/31/19.
//

#ifndef N_BODY_CONFIGURATION_H
#define N_BODY_CONFIGURATION_H

#include <string>

#include <CLI/CLI.hpp>

using std::string;


class Configuration {

public:

    Configuration(int argc, char **argv);

    unsigned int cycles;
    bool headless, verbose, debug, quiet, silent;
    string logPath, bodyArchivePath, physicsArchivePath;


};


#endif //N_BODY_CONFIGURATION_H
