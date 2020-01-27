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

        void reset();

        void attachConsole();

        void attachFile(std::string &filePath);

        void setVerbosity(std::string &level);

    };

}


#endif //N_BODY_LOGGER_H
