//
// Created by jackcamp on 1/23/20.
//

#include "Config.h"

Controller::Config::Config() :
        _commandParser{"Modular Integrator for N-body Interaction", "mini"},
        _mode{Config::Mode::INTERACTIVE} {

    _commandParser.add_option("--mode", _mode, "Set the graphics mode of the program")
            ->transform(CLI::CheckedTransformer{ModeNameMap, CLI::ignore_case});

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
            return runHeadless();
        case Mode::VIEWER :
            return runViewer();
        case Mode::INTERACTIVE :
            return runInteractive();
    }
    return EXIT_SUCCESS;
}

int Controller::Config::runHeadless() {
    return 0;
}

int Controller::Config::runViewer() {
    return 1;
}

int Controller::Config::runInteractive() {
    return 2;
}
