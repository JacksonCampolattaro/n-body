//
// Created by jackcamp on 1/23/20.
//

#ifndef N_BODY_CONFIG_H
#define N_BODY_CONFIG_H

#include <cstdlib>
#include <CLI/CLI.hpp>
#include <map>
#include <string>

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
         * @brief Constructor that initializes all options to default values.
         */
        Config();

        /**
         * @brief Runs the program after parsing the command line arguments passed in.
         *
         * @param argc The number of arguments passed to the program.
         * @param argv An array of strings, representing each argument.
         * @return The exit code of the program (e.g. 0 for success).
         */
        int run(int argc, char** argv);

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
        std::map<std::string, Mode> ModeMap {
                {"headless", Mode::HEADLESS},
                {"viewer", Mode::VIEWER},
                {"interactive", Mode::INTERACTIVE}
        };

        CLI::App _commandParser;
        Config::Mode _mode;

    };

}


#endif //N_BODY_CONFIG_H
