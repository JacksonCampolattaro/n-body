
#include "Controller/Controller.h"
#include "Controller/Application.h"

int main(int argc, char *argv[]) {

    Controller::Application application{};
    application.run(argc, argv);
}