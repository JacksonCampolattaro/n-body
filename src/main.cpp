
#include "Controller/Controller.h"

int main(int argc, char *argv[]) {

    Controller::Config config{};
    return config.run(argc, argv);
}