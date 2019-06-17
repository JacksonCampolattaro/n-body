//
// Created by jackcamp on 10/25/18.
//

#include <gtkmm/application.h>
#include "controller/Application.h"

int main(int argc, char **argv) {

    auto application = new Application();

    return application->run(argc, argv);

}