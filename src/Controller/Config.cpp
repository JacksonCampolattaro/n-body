//
// Created by jackcamp on 1/23/20.
//

#include "Config.h"

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
    // @todo configure the logger
    prepareLogger();

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

void Controller::Config::prepareLogger() {

    // Set the log level according to the user's request
    _logger->set_level(_logLevel);

    // Only add the console as a sink if the user didn't use silent mode
    if (!_logSilent) {

        // The console supports color and can be thread-safe
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        // It will use a simplified output format
        consoleSink->set_pattern("%^[%l]%$ %v");

        // The default color scheme is pretty good

        _logDistributor->add_sink(consoleSink);
    }

    // Only add a file as a sink if the user provided a valid path
    if ("" != _logFile) {
        _logDistributor->add_sink(std::make_shared<spdlog::sinks::basic_file_sink_mt>(_logFile));
    }

    // The log should now be set properly
    _logger->debug("Configured logger");
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
