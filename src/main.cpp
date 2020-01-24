
#include <gtkmm/application.h>
#include "Controller/Config.h"
#include <iostream>

int main(int argc, char *argv[]) {

    Controller::Config config{};
    return config.run(argc, argv);
}