//
// Created by jackcamp on 5/24/19.
//

#include "Application.h"

Application::Application() : Gtk::Application("org.gtkmm.nbody") {

}

void Application::on_activate() {

    window = new Window();

    add_window(*window);

    window->present();
}
