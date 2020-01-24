
#include <gtkmm/application.h>
#include "Controller/Config.h"
#include <iostream>

int main(int argc, char *argv[]) {

    Controller::Config config{argc, argv};
    return config.run();
}