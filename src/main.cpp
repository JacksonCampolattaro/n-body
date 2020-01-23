
#include "View/Application.h"

#include <gtkmm/application.h>
#include <gtkmm/window.h>


int main(int argc, char *argv[]) {

    auto app = View::Application::create("com.github.JacksonCampolattaro.nbody");

    Gtk::Window window;
    window.set_default_size(200, 200);

    return app->run(window, argc, argv);
}