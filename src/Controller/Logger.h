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

    namespace Logger {

        void prepare(
                bool silent = false,
                spdlog::level::level_enum logLevel = spdlog::level::info,
                std::string logFilePath = ""
        );

    };

}


#endif //N_BODY_LOGGER_H
