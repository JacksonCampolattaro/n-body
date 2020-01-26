
#include "Controller/Controller.h"
#include "Controller/Application.h"

int main(int argc, char *argv[]) {

//    Controller::Config config{};
//    return config.run(argc, argv);
    Controller::Application().run(argc, argv);
}