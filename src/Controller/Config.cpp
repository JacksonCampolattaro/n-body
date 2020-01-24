//
// Created by jackcamp on 1/23/20.
//

#include "Config.h"

Controller::Config::Config(int argc, char **argv) {

    // Setting the name and description
    CLI::App CLIApplication("Modular Integrator for N-body Interaction");


}

int Controller::Config::run() {
    return EXIT_SUCCESS;
}
