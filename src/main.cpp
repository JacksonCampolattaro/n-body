
#include "Controller/Controller.h"

int main(int argc, char *argv[]) {

    auto application = Controller::Application::create();
    application->run(argc, argv);
}