//
// Created by jackcamp on 1/23/20.
//

#ifndef N_BODY_CONFIG_H
#define N_BODY_CONFIG_H

#include <cstdlib>
#include <CLI/CLI.hpp>

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
         * @brief Runs the program based on its current settings.
         *
         * @return The exit code of the program (e.g. 0 for success).
         */
        int run();

        /**
         * @brief Runs the program after parsing the command line arguments passed in.
         *
         * @param argc The number of arguments passed to the program.
         * @param argv An array of strings, representing each argument.
         * @return The exit code of the program (e.g. 0 for success).
         */
        int run(int argc, char** argv);

    private:

        CLI::App _commandParser;

    };

}


#endif //N_BODY_CONFIG_H
