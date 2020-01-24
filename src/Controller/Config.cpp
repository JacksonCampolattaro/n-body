//
// Created by jackcamp on 1/23/20.
//

#include "Config.h"

Controller::Config::Config() {}

int Controller::Config::parseCommandLine(int argc, char **argv) {

    // Setting the name and description
    CLI::App CLIApplication("Modular Integrator for N-body Interaction");

    try {
        (CLIApplication).parse((argc), (argv));
    } catch(const CLI::ParseError &e) {
        return (CLIApplication).exit(e);
    }

    return EXIT_SUCCESS;
}

int Controller::Config::run() {
    // @todo Actually create the program
    return EXIT_SUCCESS;
}

int Controller::Config::run(int argc, char** argv) {

    auto status = parseCommandLine(argc, argv);
    if (0 != status) return status;
    return run();
}
