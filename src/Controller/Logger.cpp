//
// Created by jackcamp on 1/24/20.
//

#include "Logger.h"

void Controller::Logger::prepare(bool silent, spdlog::level::level_enum logLevel, std::string logFilePath) {

    std::shared_ptr<spdlog::sinks::dist_sink_mt> logDistributor
            {std::make_shared<spdlog::sinks::dist_sink_mt>()};
    std::shared_ptr<spdlog::logger> logger
            {std::make_shared<spdlog::logger>("logger", logDistributor)};

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
    spdlog::set_default_logger(logger);
    logger->info("Configured logger");
}
