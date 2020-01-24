//
// Created by jackcamp on 1/24/20.
//

#ifndef N_BODY_LOGGER_H
#define N_BODY_LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/dist_sink.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/sinks/basic_file_sink.h>

namespace Controller {

    /**
     * @brief Namespace for free functions related to Logging
     *
     * Spdlog provides a singleton logger which can be accessed anywhere, this namespaces
     * groups functions which configure spdlog's default logger.
     */
    namespace Logger {

        /**
         * @brief Configures the global spdlog default logger.
         *
         * @param silent Nothing is printed to the console when silent is true.
         * @param logLevel The level of verbosity to include in the logs.
         * @param logFilePath The location of a file to write a log to.
         */
        void prepare(
                bool silent = false,
                spdlog::level::level_enum logLevel = spdlog::level::info,
                std::string logFilePath = ""
        );

    };

}


#endif //N_BODY_LOGGER_H
