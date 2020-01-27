//
// Created by jackcamp on 1/24/20.
//

#include "Logger.h"

void Controller::Logger::prepare(bool silent, spdlog::level::level_enum logLevel, std::string logFilePath) {

    /*spdlog::set_default_logger(logger);

    // Set the log level according to the user's request
    logger->set_level(logLevel);

    // Only add the console as a sink if the user didn't use silent mode
    if (!silent) {

        // The console supports color and can be thread-safe
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        // It will use a simplified output format
        consoleSink->set_pattern("%^[%l]%$ %v");

        logDistributor->add_sink(consoleSink);
    }

    // Only add a file as a sink if the user provided a valid path
    if ("" != logFilePath) {
        logDistributor->add_sink(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath));
    }

    // The log should now be set properly
    logger->info("Configured logger");*/
}

void Controller::Logger::reset() {

    spdlog::default_logger()->sinks().clear();
}

void Controller::Logger::attachConsole() {

    // The console supports color and can be thread-safe
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    // It will use a simplified output format
    consoleSink->set_pattern("%^[%l]%$ %v");

    // Add the console sink to the default logger
    spdlog::default_logger()->sinks().push_back(consoleSink);
}

void Controller::Logger::attachFile(std::string &filePath) {

    // Configure the create a sink that writes to that file path
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filePath);

    // Add that sink to the default logger
    spdlog::default_logger()->sinks().push_back(fileSink);
    spdlog::info("Attached log file at path: \"{}\"", filePath);
}

void Controller::Logger::setVerbosity(std::string &level) {

    // Set the log level according to the user's request
    spdlog::default_logger()->set_level(spdlog::level::from_str(level));
    spdlog::info("Set logging verbosity level: \"{}\"", level);
}
