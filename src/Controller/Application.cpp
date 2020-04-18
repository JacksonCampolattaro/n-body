//
// Created by jackcamp on 1/26/20.
//

#include "Application.h"
#include "../Model/Simulation.h"
#include "HeadlessController.h"
#include <giomm/notification.h>

Controller::Application::Application() :
        Gtk::Application {
                "com.github.JacksonCampolattaro.nbody",
                Gio::APPLICATION_HANDLES_COMMAND_LINE |
                Gio::APPLICATION_HANDLES_OPEN
        } {

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

    // TODO: Temporary solution for loading a simulation file
    add_main_option_entry(OptionType::OPTION_TYPE_FILENAME,
                          "file",
                          'f',
                          "Set the file path to load a simulation from",
                          "FILEPATH"
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
    if (std::string logfilePath; options->lookup_value("logfile", logfilePath))
        Logger::attachFile(logfilePath);

    // Run the program in headless mode if that flag is set
    if (options->contains("headless")) {
        spdlog::info("Program was run in headless mode");
        spdlog::info("Headless mode is not yet implemented");

        _controller = std::make_shared<Controller>(HeadlessController());

    }

    // Run the program in viewer mode if that flag was set
    if (options->contains("viewer")) {
        spdlog::info("Program was run in viewer mode");

        // Create a window
        spdlog::debug("Creating a window with a viewport");
        auto window = new View::ViewerWindow();

        // Attach the window to this application
        spdlog::debug("Adding window to application");
        add_window(*window);
        window->show();

        // Run the program itself
        spdlog::debug("Running the program");
    }

    // TODO Temporary
    // Attach a simulation file according to the file flag
    std::string filepath;
    if (options->lookup_value("file", filepath)) {
        _controller->loadSimulation(filepath);
    }

    // TODO Temporarily disabling interactive mode
//    spdlog::info("Program was run in interactive mode");
//
//    // Create the builder
//    spdlog::debug("Loading interface from a file");
//    auto _builder = Gtk::Builder::create_from_file("../../interface.glade");
//
//    // Retrieving the window
//    spdlog::debug("Retrieving window from builder");
//    Gtk::Window* window;
//    _builder->get_widget("window", window);
//
//    // Default to running the program in interactive mode
//    spdlog::debug("Adding window to application");
//    add_window(*window);
//    window->show();

    // Run the program itself
    spdlog::debug("Running the program");
    activate();
    return Gtk::Application::on_command_line(command_line);
}

void Controller::Application::on_activate() {
    Gtk::Application::on_activate();
}

void Controller::Application::on_open(const Gio::Application::type_vec_files &files, const Glib::ustring &hint) {

    spdlog::trace("on_open invoked");
}
