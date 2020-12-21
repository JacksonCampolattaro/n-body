//
// Created by jackcamp on 1/24/20.
//

#ifndef N_BODY_LOGGER_H
#define N_BODY_LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/dist_sink.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/sinks/basic_file_sink.h>

/**
 * @brief Namespace for free functions related to Logging
 *
 * Spdlog provides a singleton logger which can be accessed anywhere, this namespace
 * groups functions which configure spdlog's default logger.
 */
namespace Logger {

    /**
     * @brief Removes options applied to the logger.
     *
     * Removes all sinks from spdlog's default instance, to prevent the creation of duplicate sinks.
     */
    void reset();

    /**
     * @brief Attaches the console to the logger.
     *
     * The console is provided as a sink for the default logger to write logs to.
     * Logs written to the console do not include as many details as those written to files.
     */
    void attachConsole();

    /**
     * @brief Attaches an output file to the logger.
     *
     * A file path is used to create a sink for the default logger to write to.
     * @todo If a file could not be created, the issue should appear as an error in the log.
     *
     * @param filePath The path of the file to be created or added to.
     */
    void attachFile(std::string &filePath);

    /**
     * @brief Sets the level of verbosity based on a string.
     *
     * @param level The logging verbosity level requested
     */
    void setVerbosity(std::string &level);

};


#endif //N_BODY_LOGGER_H
