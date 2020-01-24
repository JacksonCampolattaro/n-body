//
// Created by jackcamp on 1/23/20.
//

#include "Config.h"
#include "Logger.h"

Controller::Config::Config() {

    _commandParser.add_option("-m,--mode", _mode, "Set the graphics mode of the program")
            ->transform(CLI::CheckedTransformer {ModeNameMap, CLI::ignore_case});

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

    // The logger is constant to all modes
    // it must be prepared just before running (after parsing)
    Logger::prepare(_logSilent, _logLevel, _logFile);

    // Run the program in the appropriate mode
    switch (_mode) {
        case Mode::HEADLESS :
            return runHeadless();
        case Mode::VIEWER :
            return runViewer();
        case Mode::INTERACTIVE :
            return runInteractive();
        default:
            return runHeadless();
    }
}

int Controller::Config::runHeadless() {
    spdlog::debug("Running program in headless mode");
    // @todo Actually create the program
    return 0;
}

int Controller::Config::runViewer() {
    spdlog::debug("Running program in viewer mode");
    // @todo Actually create the program
    return 1;
}

int Controller::Config::runInteractive() {
    spdlog::debug("Running program in interactive mode");
    // @todo Actually create the program
    return 2;
}
