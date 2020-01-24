//
// Created by jackcamp on 1/23/20.
//

#include "Config.h"

Controller::Config::Config() :
        _commandParser{"Modular Integrator for N-body Interaction", "mini"} {

    // @todo Bind more commands to the parser
}

int Controller::Config::run() {
    // @todo Actually create the program
    return EXIT_SUCCESS;
}

int Controller::Config::run(int argc, char **argv) {

    // Attempt to parse the commands passed in
    try {
        _commandParser.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return _commandParser.exit(e);
    }

    return run();
}
