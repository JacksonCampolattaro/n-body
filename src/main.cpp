

#include <gtkmm/application.h>
#include <iostream>

int onCommandLine(const Glib::RefPtr<Gio::ApplicationCommandLine> &command_line,
                  Glib::RefPtr<Gtk::Application> &app) {

    std::cout << "onCommandLine" << std::endl;

    app->activate();
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

    auto application = Gtk
            ::Application::create(
            "com.github.JacksonCampolattaro.nbody",
            Gio::APPLICATION_HANDLES_COMMAND_LINE);

    application->signal_command_line().connect(sigc::bind(sigc::ptr_fun(&onCommandLine), application), false);

    return application->run(argc, argv);
}