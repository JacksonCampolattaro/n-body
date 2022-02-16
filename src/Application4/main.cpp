#include <gtkmm.h>

#include "Application.h"

int main(int argc, char* argv[])
{
    auto application = Application::create();

    return application->run(argc, argv);
}