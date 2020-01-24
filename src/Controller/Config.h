//
// Created by jackcamp on 1/23/20.
//

#ifndef N_BODY_CONFIG_H
#define N_BODY_CONFIG_H

#include <CLI/CLI.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/dist_sink.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/sinks/basic_file_sink.h>

#include <cstdlib>
#include <string>
#include <map>

namespace Controller {

    /**
     * @brief A manager class for the program's operation status.
     *
     * This class is responsible for keeping track of the program's state, and
     * all of its settings. It contains many parameters which are set on creation,
     * but may be updated as the program runs.
     */
    class Config {

    public:

        /**
         * @brief Constructor that binds options to console commands.
         */
        Config();

        /**
         * @brief Runs the program after parsing the command line arguments passed in.
         *
         * @param argc The number of arguments passed to the program.
         * @param argv An array of strings, representing each argument.
         * @return The exit code of the program (e.g. 0 for success).
         */
        int run(int argc, char **argv);

        /**
         * @brief Runs the program based on its current settings.
         *
         * @return The exit code of the program (e.g. 0 for success).
         */
        int run();

    private:

        enum class Mode : int {
            HEADLESS,
            VIEWER,
            INTERACTIVE
        };
        std::map<std::string, Mode> ModeNameMap {
                {"headless",    Mode::HEADLESS},
                {"viewer",      Mode::VIEWER},
                {"interactive", Mode::INTERACTIVE}
        };

        CLI::App _commandParser {"Modular Integrator for N-body Interaction", "mini"};

        std::shared_ptr<spdlog::sinks::dist_sink_mt> _logDistributor
                {std::make_shared<spdlog::sinks::dist_sink_mt>()};
        std::shared_ptr<spdlog::logger> _logger
                {std::make_shared<spdlog::logger>("logger", _logDistributor)};
        std::string _logFile {""};
        spdlog::level::level_enum _logLevel {spdlog::level::debug};
        bool _logSilent {false};

        Config::Mode _mode {Config::Mode::INTERACTIVE};

        /**
         * @brief Sets up the logger as specified by other member variables.
         */
        void prepareLogger();

        /**
         * @brief Runs the program in the console only.
         *
         * @return The exit code of the program (e.g. 0 for success).
         */
        int runHeadless();

        /**
         * @brief Runs the program with a live view of the results.
         *
         * @return The exit code of the program (e.g. 0 for success).
         */
        int runViewer();

        /**
         * @brief Runs the program with a full graphical interface.
         *
         * @return The exit code of the program (e.g. 0 for success).
         */
        int runInteractive();

    };

}


#endif //N_BODY_CONFIG_H
