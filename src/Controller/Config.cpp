//
// Created by jackcamp on 1/23/20.
//

#include "Config.h"

Controller::Config::Config() :
        _commandParser{"Modular Integrator for N-body Interaction", "mini"},
        _mode{Config::Mode::INTERACTIVE} {

    _commandParser.add_option("--mode", _mode, "Set the graphics mode of the program")
            ->transform(CLI::CheckedTransformer{ModeMap, CLI::ignore_case});

    // @todo Bind more commands to the parser
}

int Controller::Config::run(int argc, char **argv) {

    // Attempt to parse the commands passed in
    try {
        _commandParser.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return _commandParser.exit(e);
    }

    // Run the program if the commands were successfully parsed
    return run();
}

int Controller::Config::run() {

    // @todo Actually create the program

    switch (_mode) {
        case Mode::HEADLESS :
            return 0;
        case Mode::VIEWER :
            return 1;
        case Mode::INTERACTIVE :
            return 2;
    }

}
