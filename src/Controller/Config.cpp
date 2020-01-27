//
// Created by jackcamp on 1/23/20.
//

#include "Config.h"
#include "Logger.h"

Controller::Config::Config() {

    // The program's mode can be set to the following keywords
    std::map<std::string, Mode> modeMap {
            {"headless",    Mode::HEADLESS},
            {"viewer",      Mode::VIEWER},
            {"interactive", Mode::INTERACTIVE}
    };
    _commandParser.add_option("-m,--mode", _mode, "Set the graphics mode of the program")
            ->transform(CLI::CheckedTransformer {modeMap, CLI::ignore_case});

    // The verbosity of the program can be set to the following keywords
    std::map<std::string, spdlog::level::level_enum> logLevelMap {
            {"trace",    spdlog::level::trace},
            {"debug",    spdlog::level::debug},
            {"info",     spdlog::level::info},
            {"warn",     spdlog::level::warn},
            {"error",    spdlog::level::err},
            {"critical", spdlog::level::critical}
    };
    _commandParser.add_option("-v,--verbosity", _logLevel, "Set the verbosity level of the program")
            ->transform(CLI::CheckedTransformer {logLevelMap, CLI::ignore_case});

    // The program can be run in silent mode with a specific flag
    _commandParser.add_flag("-s,--silent",
                            _logSilent,
                            "Disables printing any information to the console"
    );

    // The program can be set to write logs to a specific file
    _commandParser.add_option("-l,--logfile",
                              _logFile,
                              "Sets the path to write log files to",
                              true
    );

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
