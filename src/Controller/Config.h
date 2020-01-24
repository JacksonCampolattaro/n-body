//
// Created by jackcamp on 1/23/20.
//

#ifndef N_BODY_CONFIG_H
#define N_BODY_CONFIG_H

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
         * @brief Constructor that parses command line arguments to create a configuration.
         *
         * @param argc The number of arguments passed to the program.
         * @param argv An array of strings, representing each argument.
         */
        Config(int argc, char *argv[]);

        /**
         * @brief Runs the program based on the Config's settings.
         *
         * @return The Exit Code of the program (e.g. 0 for success).
         */
        int run();

    private:


    };

}


#endif //N_BODY_CONFIG_H
