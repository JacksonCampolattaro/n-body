//
// Created by jackcamp on 1/23/20.
//

#include "Config.h"

Controller::Config::Config() {}

int Controller::Config::parseCommandLine(int argc, char **argv) {

    // Setting the name and description
    CLI::App CLIApplication("Modular Integrator for N-body Interaction");

    return EXIT_SUCCESS;
}

int Controller::Config::run() {
    // @todo Actually create the program
}

int Controller::Config::run(int argc, char** argv) {

    parseCommandLine(argc, argv);

    return run();
}
