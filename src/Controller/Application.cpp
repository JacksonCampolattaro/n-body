//
// Created by jackcamp on 1/26/20.
//

#include "Application.h"

Controller::Application::Application() :
        Gtk::Application {"my.app", Gio::APPLICATION_HANDLES_COMMAND_LINE | Gio::APPLICATION_HANDLES_OPEN} {

    add_main_option_entry(OptionType::OPTION_TYPE_BOOL,
                          "silent",
                          's',
                          "Disables printing any information to the console"
    );

    add_main_option_entry(OptionType::OPTION_TYPE_FILENAME,
                          "verbosity",
                          'v',
                          "Set the verbosity level of the program's logging",
                          "{trace|debug|info|warn|error|critical}"
    );

    add_main_option_entry(OptionType::OPTION_TYPE_FILENAME,
                          "logfile",
                          'l',
                          "Set the file path to write a log file to",
                          "FILEPATH"
    );

    add_main_option_entry(OptionType::OPTION_TYPE_BOOL,
                          "headless",
                          '\0',
                          "Runs the program without any interface or visualization"
    );

    add_main_option_entry(OptionType::OPTION_TYPE_BOOL,
                          "viewer",
                          '\0',
                          "Runs the program with only a visualization and no interactivity"
    );
}

int Controller::Application::on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine> &command_line) {
    auto options = command_line->get_options_dict();

    // Clear all sinks from the logger
    Logger::reset();

    // Only attach the console to the logger if the silent flag isn't set
    if (!options->contains("silent"))
        Logger::attachConsole();

    // Set the log level based on the verbosity flag
    if (std::string verbosity; options->lookup_value("verbosity", verbosity))
        Logger::setVerbosity(verbosity);

    // Attach a log file according to the logfile flag
    if (std::string filepath; options->lookup_value("logfile", filepath))
        Logger::attachFile(filepath);

    // Run the program in headless mode if that flag is set
    if (options->contains("headless")) {
        spdlog::info("Running in headless mode");
        return Gtk::Application::on_command_line(command_line);
    }

    activate();
    return Gtk::Application::on_command_line(command_line);
}

void Controller::Application::on_startup() {
    Gtk::Application::on_startup();
}

void Controller::Application::on_activate() {
    Gtk::Application::on_activate();
    spdlog::info("activate");

    add_window(_window);
    _window.show();

}
