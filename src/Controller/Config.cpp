//
// Created by jackcamp on 1/23/20.
//

#include "Config.h"

Controller::Config::Config() :
        _commandParser{"Modular Integrator for N-body Interaction", "mini"},
        _logDistributor{std::make_shared<spdlog::sinks::dist_sink_mt>()},
        _logger{std::make_shared<spdlog::logger>("logger", _logDistributor)},
        _mode{Config::Mode::INTERACTIVE} {

    _logDistributor->add_sink(std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>());
    _logger->error("Console added as log sink");

    _commandParser.add_option("-m,--mode", _mode, "Set the graphics mode of the program")
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

    // Run the program in the appropriate mode
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
    // @todo Actually create the program
    return 0;
}

int Controller::Config::runViewer() {
    // @todo Actually create the program
    return 1;
}

int Controller::Config::runInteractive() {
    // @todo Actually create the program
    return 2;
}
