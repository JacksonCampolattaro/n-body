//
// Created by jackcamp on 5/24/19.
//

#include "nbody_application.h"

nbody_application::nbody_application() : Gtk::Application("org.gtkmm.nbody") {

}

void nbody_application::on_activate() {

    auto window = new nbody_window();

    add_window(*window);

    window->present();
}
