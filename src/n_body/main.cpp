//
// Created by jackcamp on 10/25/18.
//

#include <gtkmm/application.h>
#include "Launcher/Launcher.h"
#include "Gtkmm/nbody_application.h"

int main(int argc, char **argv) {

    /*auto application = Gtk::Application::create(argc, argv, "org.gtkmm.n_body");
    Launcher launcher;
    application->run(launcher);*/

    auto application = new nbody_application();

    return application->run(argc, argv);

}